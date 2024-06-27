#pragma once
#include "../Общие зависимости/NetworkHandler.h"

class ClientServer : public NetworkHandler
{
private:
    std::vector<char> printScreen();
    std::string image_size_data;
    size_t image_size_data_size;

    DataType send_data_type;

    virtual void handlerData(DataBuffer data, Client& client) final;

    virtual void handlerConnect(Client& client);

    virtual void handlerDisconnect(Client& client);

    virtual bool sendDataBy(uint32_t host, uint16_t port, const void* buffer, const size_t size) override;

public:
    ClientServer(const uint16_t port);
};

