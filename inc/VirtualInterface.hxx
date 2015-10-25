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

#ifndef _VIRTUAL_INTERFACE_HXX
#define _VIRTUAL_INTERFACE_HXX

#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

class Tunnel;

class VirtualInterface
{
    public:
        VirtualInterface(Tunnel *parent, std::string const & device_name);
        const int virtual_interface_fd();

    protected:
        void create_virtual_interface();

        Tunnel *m_parent;
        std::string m_device_name;
        int m_virtual_interface_fd;
};

#endif

