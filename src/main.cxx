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

#include <cstdlib>
#include <getopt.h>

#include "Tunnel.hxx"

#define VERSION 0.1

void banner();
void usage(std::string const & prog_name);

int main(int argc, char *argv[])
{
    std::string device_name("tap0");
    unsigned short port = 80;

    int opt = 0;
    int long_index =0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"interface", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    banner();

    while ((opt = getopt_long(argc, argv,"hi:p:", long_options, &long_index)) != -1)
    {
        switch (opt) {
            case 'h':
                usage(argv[0]); 
                exit(0);
            case 'i' :
                device_name = optarg;
                break;
            case 'p' :
                port = (unsigned short) strtoul(optarg, NULL, 10);
                break;
            default:
                usage(argv[0]); 
                exit(1);
        }
    }

    if (argc > optind)
    {
        std::cerr << argv[0] << ": too many arguments" << std::endl;
        usage(argv[0]);
        exit(1);
    }

    Tunnel *tun = new Tunnel(device_name, port);
    tun->perform();

    return 0;
}

void banner()
{
    std::cout << " _____           _   _   _       _     _ " << std::endl;
    std::cout << "|  ___|__   ___ | |_| | | | ___ | | __| |" << std::endl;
    std::cout << "| |_ / _ \\ / _ \\| __| |_| |/ _ \\| |/ _` |" << std::endl;
    std::cout << "|  _| (_) | (_) | |_|  _  | (_) | | (_| |" << std::endl;
    std::cout << "|_|  \\___/ \\___/ \\__|_| |_|\\___/|_|\\__,_|";
    std::cout << "v" << VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "Yannick Méheut <yannick (at) meheut (dot) org> - Copyright © 2015" << std::endl;
    std::cout << std::endl;
}

void usage(std::string const & prog_name)
{
    std::cerr << "Usage: " << prog_name << " [-h|--help] [(-i|--interface) interface_name] [(-p|--port) port]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "\t-h, --help\tPrints this message" << std::endl;
    std::cerr << "\t-i, --interface\tName of the virtual interface to" << std::endl;
    std::cerr << "\t\t\tcreate/attach to (default: tap0)" << std::endl;
    std::cerr << "\t-p, --port\tPort to listen on (default: 80)" << std::endl;
}

