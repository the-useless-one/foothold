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

#ifndef _COMMUNICATOR_HXX
#define _COMMUNICATOR_HXX

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <cerrno>

class Tunnel;

class Communicator
{
    public:
        Communicator(Tunnel *parent, unsigned short port);
        void wait_for_client();
        void perform();

    protected:
        void create_socket();

        Tunnel *m_parent;
        unsigned short m_port;
        int m_socket_fd;
        int m_client_socket_fd;
};

#endif

