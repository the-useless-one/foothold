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

#include "Tunnel.hxx"

Tunnel::Tunnel(std::string const & device_name, unsigned short port) :
    m_communicator(new Communicator(this, port)),
    m_virtual_interface(new VirtualInterface(this, device_name))
{
}

const int Tunnel::virtual_interface_fd()
{
    return m_virtual_interface->virtual_interface_fd();
}

void Tunnel::perform()
{
    while (true)
    {
        m_communicator->wait_for_client();
        m_communicator->perform();
    }
}

