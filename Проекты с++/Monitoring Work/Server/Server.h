#pragma once
#include "../Общие зависимости/NetworkHandler.h"

class Server : public NetworkHandler
{
private:
    void SaveBitMapToClipboard(HBITMAP& hBitmap);
    
    virtual void handlerData(DataBuffer& data, Client& client) override;

    void sendMsg();

    void setUserData(Client& client);

    int width, height, planes, bits_pixel;
public:
    Server(const uint16_t port);

    void printClientList();

    void userInput();
};


