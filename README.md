# FootHold
     ______          _   _    _       _     _ 
    |  ____|        | | | |  | |     | |   | |
    | |__ ___   ___ | |_| |__| | ___ | | __| |
    |  __/ _ \ / _ \| __|  __  |/ _ \| |/ _` |
    | | | (_) | (_) | |_| |  | | (_) | | (_| |
    |_|  \___/ \___/ \__|_|  |_|\___/|_|\__,_|

Using code execution on a remote computer as a foothold to the LAN.

## HISTORY

Ever since I started doing phishing campaign in the context of my work, I've
been intrigued about using our remote code execution on our victim's computer
as a foothold to their LAN.

I decided to gather information on pivoting, so that I could implement my own
solution. I found a tremendous amount of information in [this article by 
Raphael Mudge](http://blog.cobaltstrike.com/2014/10/14/how-vpn-pivoting-works-with-source-code/),
and [from this article by David Brini](http://backreference.org/2010/03/26/tuntap-interface-tutorial/).

These articles allowed me to create the first part of my project: FootHold.

This program is the server part of a client/server tunnel between a compromised
computer (client) and the attacker's machine (server). There is no client
part for now, but this program was tested against the `simpletun` program
writtent by David Brini.

The server creates a virtual TAP interface, and listens on a TCP socket. The
client listens on a local interface, and connects to the server. Everything
read on the virtual interface is sent to the client and injected in the local
interface. Everything read from the client's local interface is sent to the
server and written to the virtual interface.

There is no encryption of the communication between the server and the client
for the moment (cf. **TODO** section).

## DISCLAIMER

This program was designed without any malevolent intention in mind. Don't use
it for any illegal activity. I'm not responsible for the actions of the users
of this program.

## REQUIREMENTS

This program uses `cmake` to generate its `Makefile`s. You need to install
`cmake`, and then issue the command:

    cmake .

in the `foothold` directory to generate a `Makefile`.

You can then build the program with `make`.

## USAGE

Just go to the `foothold` directory and type the following command:

    # ./foothold

To see a list of the options, just issue:

    # ./foothold --help
     _____           _   _   _       _     _ 
    |  ___|__   ___ | |_| | | | ___ | | __| |
    | |_ / _ \ / _ \| __| |_| |/ _ \| |/ _` |
    |  _| (_) | (_) | |_|  _  | (_) | | (_| |
    |_|  \___/ \___/ \__|_| |_|\___/|_|\__,_|v0.1

    Yannick Méheut <yannick (at) meheut (dot) org> - Copyright © 2015

    Usage: ./foothold [-h|--help] [(-i|--interface) interface_name] [(-p|--port) port]

            -h, --help      Prints this message
            -i, --interface Name of the virtual interface to
                            create/attach to (default: tap0)
            -p, --port      Port to listen on (default: 80)

**NB:** the program has to run as root (or with sudo) so that it can create the
virtual interface. I plan on implementing a change of owner of the interface
and a dropping of privileges so that the program doesn't have to run as root
(cf. **TODO** section).

You also, of course, need to run it as root to listen on a privileged port.

## TODO

* Implement virtual interface ownership change and dropping of privileges
* Implement other communicator than TCP socket (UDP socket, ICMP communicator,
  DNS communicator, HTTP communicator, etc.)
* Implement encryption of the communication

## COPYRIGHT

FootHold - Using code execution on a remote computer as a foothold to the LAN

Yannick Méheut [yannick (at) meheut (dot) org] - Copyright © 2015

This program is free software: you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation, either version 3 of the License, or (at your 
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program. If not, see
[http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).

