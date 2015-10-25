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

#ifndef _TUNNEL_HXX
#define _TUNNEL_HXX


#include "VirtualInterface.hxx"
#include "Communicator.hxx"

class Tunnel
{
    public:
        Tunnel(std::string const & device_name, unsigned short port);
        const int virtual_interface_fd();
        void perform();

    protected:
        void create_virtual_interface();

        VirtualInterface *m_virtual_interface;
        Communicator *m_communicator;
};

#endif

