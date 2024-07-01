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

void Server::handlerData(DataBuffer& data, Client& client)
{
    std::string data_str = dataToStr(data);
    if (data_str.empty()) return;

    switch (data.type)
    {
    case DataType::message:
    {
        std::cout << "\nMessage received:\n";
        std::cout << "ip: " << ipToStr(client.getHost())  << std::endl;
        std::cout << "msg: " << data_str << std::endl;
        break;
    }
    case DataType::image:
    {
        BITMAPINFOHEADER bih;
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = planes;
        bih.biBitCount = bits_pixel;
        bih.biCompression = BI_RGB;
        bih.biSizeImage = 0; // Можно установить в 0 для BI_RGB
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        BITMAPINFO bi;
        bi.bmiHeader = bih;

        size_t data_size = data.size - sizeof(DataType);
        void* sourcePtr = malloc(data_size);
        memcpy(sourcePtr, static_cast<uint8_t*>(data.data_ptr) + sizeof(DataType), data_size);

        HDC hdc = GetDC(NULL);
        HBITMAP hBitmap = CreateDIBitmap(hdc, &bih, CBM_INIT, sourcePtr, &bi, DIB_RGB_COLORS);
        ReleaseDC(NULL, hdc);
        SaveBitMapToClipboard(hBitmap);
        DeleteObject(hBitmap);
        free(sourcePtr);
        std::cout << "\nPrint screen received:\n";
        std::cout << "ip: " << ipToStr(client.getHost())  << std::endl;
        break;
    }
    case DataType::image_data:
    {
        std::istringstream iss(data_str);
        iss >> width >> height >> planes >> bits_pixel;
        break;
    }
    case DataType::user_data:
    {
        std::istringstream iss(data_str);
        iss >> client.user_name;
        iss >> client.computer_name;
        iss >> client.work_group;
        setUserData(client);
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

void Server::setUserData(Client& client)
{
    for (int i = 0; i < client_all.size(); ++i)
    {
        if (client.getHost() == client_all[i].address.sin_addr.S_un.S_addr && client.getPort() == client_all[i].address.sin_port)
        {
            client_all[i].user_name = client.user_name;
            client_all[i].computer_name = client.computer_name;
            client_all[i].work_group = client.work_group;
            break;
        }
    }
}

Server::Server(const uint16_t port) : NetworkHandler(port)
{

}

void Server::printClientList()
{
    int i = 0;
    std::cout << std::endl;
    for (ClientData& client : client_all) {
        std::cout << i << ":{\n";
        std::cout << "ip: " << ipToStr(client.address.sin_addr.S_un.S_addr) << "\n";
        std::cout << "user name: " << client.user_name << "\n";
        std::cout << "computer name: " << client.computer_name << "\n";
        std::cout << "work group: " << client.work_group << "\n";
        for (const std::unique_ptr<Client>& client_c : client_list)
        {
            if (client_c->getHost() == client.address.sin_addr.S_un.S_addr && client_c->getPort() == client.address.sin_port)
            {
                client.updateLastTimeActive();
                break;
            }
        }
        std::cout << "last active: " << client.getLastTimeActive() << "\n}" << std::endl;
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
