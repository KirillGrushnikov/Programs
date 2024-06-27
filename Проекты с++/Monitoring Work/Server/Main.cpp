#include "Server.h"
#include <string>

int main(int argc, char* argv[])
{
    uint16_t port = 1234;
    Server server(port);


    if (server.start() == ServerStatus::up) {
        std::cout << "Server is active!" << std::endl;
        std::thread th = std::thread([&server] {server.userInput(); });
        th.join();
        server.joinTheads();
    }
    else
    {
        std::cout << "Server start error! Error: " << server.getStatusStr() << std::endl;
        return -1;
    }
}

