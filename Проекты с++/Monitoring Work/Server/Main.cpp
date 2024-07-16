#include "Server.h"
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    uint16_t port = 56091;

    std::ofstream file("log.txt");
    std::streambuf* oldCoutBuffer = std::cout.rdbuf();
    std::streambuf* fileBuffer = file.rdbuf();
    std::streambuf* coutBuffer = std::cout.rdbuf();
    dualbuf dualBuffer(coutBuffer, fileBuffer);
    std::cout.rdbuf(&dualBuffer);

    std::cout << "Server app is start!" << std::endl;
    std::cout << "Enter port: ";
    std::cin >> port;
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

    std::cout.rdbuf(oldCoutBuffer);
    file.close();
}

