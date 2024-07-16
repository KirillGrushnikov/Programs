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
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    uint16_t port = rand() % 20000;
    uint16_t server_port = 56091;
    std::string ip = "127.0.0.1";

    std::cout << "Client app is start!" << std::endl;
    std::cout << "Enter port: ";
    std::cin >> server_port;
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
        c_server.sendUserData();
        c_server.joinTheads();
    }
    else
    {
        std::cout << "Client start error! Error: " << c_server.getStatusStr() << std::endl;
        return -1;
    }
}