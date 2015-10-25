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

#include "VirtualInterface.hxx"
#include "Tunnel.hxx"

VirtualInterface::VirtualInterface(Tunnel *parent, std::string const & device_name) :
    m_parent(parent), m_device_name(device_name)
{
    std::cout << "[ ] Creating virtual interface";
    std::cout.flush();
    try
    {
        create_virtual_interface();
    }
    catch (std::exception & e)
    {
        std::cout << "\r[!]" << std::endl;
        std::cerr << e.what() << std::endl;
        exit(2);
    }
    std::cout << "\r[+] Creating virtual interface: " << m_device_name << std::endl;
}

void VirtualInterface::create_virtual_interface()
{
    struct ifreq ifr;
    int fd;

    // We open the clone device to create our virtual interface
    if ( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    {
        throw std::runtime_error("Error: can't open clone device /dev/net/tun");
    }

    // We prepare the struct ifr
    bzero(&ifr, sizeof(ifr));

    // TODO
    // Pass the flags by argument
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if (!m_device_name.empty())
    {
        strncpy(ifr.ifr_name, m_device_name.c_str(), IFNAMSIZ);
    }

    // We create the virtual interface
    if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0)
    {
        close(fd);
        throw std::runtime_error("Error: can't create virtual interface (are you running as root?)");
    }

    // We get the name of our newly created interface
    m_device_name = ifr.ifr_name;

    // We get the file descriptor to our newly created interface
    m_virtual_interface_fd = fd;

    //if (ioctl(m_virtual_interface_fd, TUNSETPERSIST, 1) < 0){
        //return;
    //}
}

const int VirtualInterface::virtual_interface_fd()
{
    return m_virtual_interface_fd;
}

