#include "Server.h"
#include <sstream>
#include <functional>

void Server::SaveBitMapToClipboard(HBITMAP& hBitmap)
{
    if (OpenClipboard(NULL))
    {
        EmptyClipboard(); 
        if (!SetClipboardData(CF_BITMAP, hBitmap))
        {
            std::cerr << "Не удалось поместить bitmap в буфер обмена";
        }
        CloseClipboard(); 
    }
}

void Server::handlerData(DataBuffer data, Client& client)
{
    std::string data_str = dataToStr(data);
    if (data_str.empty())
        return;

    switch (data.type)
    {
    case DataType::message:
    {
        std::cout << "\nMessage received:\n";
        std::cout << "ip: " << ipToStr(client.getHost()) << ":" << ntohs(client.getPort()) << std::endl;
        std::cout << "msg: " << data_str << std::endl;
        break;
    }
    case DataType::image:
    {
        BITMAPINFOHEADER bih;
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = 1;
        bih.biBitCount = 32;
        bih.biCompression = BI_RGB;
        bih.biSizeImage = 0; // Можно установить в 0 для BI_RGB
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        BITMAPINFO bi;
        bi.bmiHeader = bih;

        void* bitmap_ptr = nullptr;
        void* sourcePtr = static_cast<char*>(data.data_ptr) + sizeof(DataType);
        size_t size = data.size - sizeof(DataType);
        HDC hdc = GetDC(NULL);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &bitmap_ptr, NULL, 0);
        memcpy(bitmap_ptr, sourcePtr, size);
        ReleaseDC(NULL, hdc);
        SaveBitMapToClipboard(hBitmap);
        DeleteObject(hBitmap);
        break;
    }
    case DataType::image_size:
    {
        std::istringstream iss(data_str);
        iss >> width >> height;
        break;
    }
    }
}

void Server::sendMsg()
{
    char* c_msg = new char[150];
    std::cin.getline(c_msg, 150);
    std::string msg(c_msg);
    msg.erase(msg.begin());
    sendData(msg.c_str(), msg.size());
}

Server::Server(const uint16_t port) : NetworkHandler(port)
{

}

void Server::printClientList()
{
    int i = 0;
    std::cout << std::endl;
    for (ClientTime& client : client_all) {
        std::cout << i << ": ip: " << ipToStr(client.address.sin_addr.S_un.S_addr) << ":" << client.address.sin_port;
        for (const std::unique_ptr<Client>& client_c : client_list)
        {
            if (client_c->getStatus() == SocketStatus::connected && client_c->getHost() == client.address.sin_addr.S_un.S_addr && client_c->getPort() == client.address.sin_port)
            {
                client.updateLastTimeActive();
                break;
            }
        }
        std::cout << " " << client.getLastTimeActive() << std::endl;
        i += 1;
    }
}

void Server::userInput()
{
    std::vector<std::vector<std::string>> full_commands = { {"stop", "e", "exit"}, {"disconnect", "d"}, {"list", "l"}, {"send"}};
    typedef void(Server::*function)();
    std::vector<function> commands = { &Server::stop, &Server::disconnectAll, &Server::printClientList, &Server::sendMsg };

    while (_status == ServerStatus::up)
    {
        std::cout << ">>> ";
        std::string request;
        std::cin >> request;
        for (int i = 0; i < full_commands.size(); ++i)
        {
            for (int j = 0; j < full_commands[i].size(); ++j)
            {
                if (request == full_commands[i][j])
                {
                    (this->*commands[i])();
                    break;
                }
            }
        }
    }
}
