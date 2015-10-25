/*
    Yannick Méheut <yannick (at) meheut (dot) org> - Copyright © 2015
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Communicator.hxx"
#include "Tunnel.hxx"

Communicator::Communicator(Tunnel *parent, unsigned short port) : m_parent(parent),
    m_port(port)
{
    std::cout << "[ ] Creating TCP socket";
    std::cout.flush();
    try
    {
        create_socket();
    }
    catch (std::exception & e)
    {
        std::cout << "\r[!]" << std::endl;
        std::cerr << e.what() << std::endl;
        exit(3);
    }
    std::cout << "\r[+] Creating TCP socket: listening on " << m_port << std::endl;
}

void Communicator::create_socket()
{
    int yes = 1;
    struct sockaddr_in server_addr;

    // We create a socket
    if ((m_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw std::runtime_error("Error: can't create socket");
    }

    if (setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 )
    {
        throw std::runtime_error("Error: can't set socket as reusable");
    }

    bzero(&server_addr, sizeof(server_addr));

    // We bind the socket to the given port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(m_port);
    if (bind(m_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        throw std::runtime_error("Error: can't bind socket");
    }

    // We listen on the socket
    if (listen(m_socket_fd, 5) < 0)
    {
        throw std::runtime_error("Error: can't listen on socket");
    }
}

void Communicator::wait_for_client()
{
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    // We wait for a client to connect
    std::cout << "[ ] Waiting for client";
    std::cout.flush();
    m_client_socket_fd = accept(m_socket_fd,
            (struct sockaddr *) &client_address,
            &client_length);

    if (m_client_socket_fd < 0)
    {
        std::cout << "\r[!]" << std::endl;
        throw std::runtime_error("Error: can't accept connection from client");
    }
    else
    {
        std::cout << "\r[+] Waiting for client: got connection from ";
        std::cout << inet_ntoa(client_address.sin_addr) << std::endl;
    }

}

void Communicator::perform()
{
    unsigned char buffer[8192];
    ssize_t read_len = 0, write_len = 0;
    uint16_t packet_len = 0;


    while (true)
    {
        fd_set socket_set;
        int max_fd;

        // We define a set of sockets from which we will asynchronously
        // wait for data we can read. This set contains the file descriptor
        // to the virtual interface and the file descriptor of the socket
        // from the client
        FD_ZERO(&socket_set);
        FD_SET(m_parent->virtual_interface_fd(), &socket_set);
        FD_SET(m_client_socket_fd, &socket_set);

        max_fd = (m_parent->virtual_interface_fd() > m_client_socket_fd) ? m_parent->virtual_interface_fd() : m_client_socket_fd;

        // We wait for data to be available from our sockets
        if (select(max_fd + 1, &socket_set, NULL, NULL, NULL) < 0)
        {
            throw std::runtime_error("Error: can't read from sockets");
        }

        // If there is data from our virtual interface
        if (FD_ISSET(m_parent->virtual_interface_fd(), &socket_set))
        {
            if ((read_len = read(m_parent->virtual_interface_fd(), buffer, 8192)) < 0)
            {
                throw std::runtime_error("Error: can't read from virtual interface");
            }
            else if (read_len == 0)
            {
                continue;
            }
            std::cout << "TAP2NET: read " << read_len << " bytes" << std::endl;

            packet_len = htons(read_len);
            write_len = write(m_client_socket_fd, (char *)&packet_len, sizeof(packet_len));
            std::cout << "TAP2NET: wrote " << write_len << " bytes" << std::endl;
            write_len = write(m_client_socket_fd, buffer, read_len);
            std::cout << "TAP2NET: wrote " << write_len << " bytes" << std::endl;
        }

        // If there is data from our client's socket
        if (FD_ISSET(m_client_socket_fd, &socket_set))
        {
            if ((read_len = read(m_client_socket_fd, (char *)&packet_len, sizeof(packet_len))) < 0)
            {
                throw std::runtime_error("Error: can't read from client interface");
            }
            else if (read_len == 0)
            {
                std::cout << "Client closed connection" << std::endl;
                break;
            }
            read_len = read(m_client_socket_fd, buffer, ntohs(packet_len));

            std::cout << "NET2TAP: read " << read_len << " bytes" << std::endl;

            std::cout << "NET2TAP: wrote ";
            std::cout << write(m_parent->virtual_interface_fd(), buffer, read_len);
            std::cout << " bytes" << std::endl;
        }

    }
}

