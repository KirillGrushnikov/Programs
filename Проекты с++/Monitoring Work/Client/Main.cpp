#include "ClientServer.h"
using namespace std::chrono_literals;

void checkPort()
{
    u_short port_host_order = 27015;
    std::cout << "Source port: " << port_host_order << std::endl;

    u_short port_network_order = htons(port_host_order);
    std::cout << "Port in network byte order: " << port_network_order << std::endl;

    // ќбратное преобразование в пор€док байтов хоста
    u_short port_converted_back = ntohs(port_network_order);
    std::cout << "Port converted back: " << port_converted_back << std::endl;

    // ѕроверка на соответствие
    if (port_host_order == port_converted_back) {
        std::cout << "Conversion done correctly." << std::endl;
    }
    else {
        std::cerr << "Conversion error." << std::endl;
    }
}

int main(int argc, char* argv[])
{
    uint16_t port = 1111;
    uint16_t server_port = 1234;

    std::string ip;
    std::cout << "Enter server-ip: ";
    std::cin >> ip;

    std::cout << "Enter server-port: ";
    std::cin >> server_port;

    std::cout << "Enter client-port: ";
    std::cin >> port;
    ClientServer c_server(port);



    if (c_server.start() == ServerStatus::up) {
        std::cout << "Client is active!" << std::endl;
        uint32_t host;
        inet_pton(AF_INET, ip.c_str(), &host);
        while(!c_server.connectToServer(host, server_port))
        {
            std::this_thread::sleep_for(500ms);
            std::cout << "connected failed\r";
        }
        c_server.joinTheads();
    }
    else
    {
        std::cout << "Client start error! Error: " << c_server.getStatusStr() << std::endl;
        return -1;
    }
}