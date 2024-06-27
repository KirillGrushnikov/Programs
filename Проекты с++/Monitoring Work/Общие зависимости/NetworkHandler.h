#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <ctime>

#include "general.h"


// Коды статуса сервера
enum class ServerStatus : uint8_t {
    up = 0,
    err_socket_init = 1,
    err_socket_bind = 2,
    err_socket_listening = 3,
    close = 4
};

class NetworkHandler
{
protected:
    class Client;

    struct ClientTime;

    static NetworkHandler* instance;

    static BOOL WINAPI ConsoleHandler(DWORD signal);

    WSAData w_data;

    int serverSocket;
    volatile ServerStatus _status = ServerStatus::close;

    std::thread newConnectionHandler_thread;
    std::thread waitingData_thread;
    std::mutex client_mutex;

    uint16_t port;
    
    std::string dataToStr(const DataBuffer& data);

    std::string ipToStr(uint32_t host);

    virtual void handlerData(DataBuffer data, Client& client);

    virtual void handlerConnect(Client& client);

    virtual void handlerDisconnect(Client& client);

    std::list<std::unique_ptr<Client>> client_list;
    std::vector<ClientTime> client_all;

    void addUser(sockaddr_in client);

    void waitingData();

    void handlerNewConnection();

public:
    NetworkHandler(const uint16_t port);
    virtual ~NetworkHandler();

    bool connectToServer(uint32_t host, uint16_t port);

    ServerStatus start();

    void stop();

    void joinTheads();

    ServerStatus getStatus() const;

    std::string getStatusStr() const;

    // Отправить данные всем клиентам сервера
    virtual void sendData(const void* buffer, const size_t size);

    // Отправить данные клиенту по порту и хосту
    virtual bool sendDataBy(uint32_t host, uint16_t port, const void* buffer, const size_t size);

    // Отключить клиента по порту и хосту
    bool disconnectBy(uint32_t host, uint16_t port);

    // Отключить всех клиентов
    void disconnectAll();
};

struct NetworkHandler::ClientTime
{
    tm* last_active_time;

    sockaddr_in address;

    uint16_t local_port;

    std::string getLastTimeActive();

    void updateLastTimeActive();

    ClientTime(sockaddr_in address);
};

class NetworkHandler::Client : public ClientBase {
    friend class NetworkHandler;

    sockaddr_in address;

    int socket;

    SocketStatus _status = SocketStatus::connected;
public:

    Client(int socket, sockaddr_in address);

    virtual ~Client() override;

    virtual uint32_t getHost() const override;

    virtual uint16_t getPort() const override;

    virtual SocketStatus getStatus() const override { return _status; }

    virtual SocketStatus disconnect() override;

    // Получить данные от клиента
    virtual DataBuffer loadData() override;

    // Отправить данные клиенту
    virtual bool sendData(const void* buffer, const size_t size, DataType data_type) const override;

    // Определить "сторону" клиента
    virtual SocketType getType() const override { return SocketType::server_socket; }

    bool operator ==(Client& a)
    {
        return this->getHost() == a.getHost() && this->getPort() == a.getPort();
    }

    bool operator !=(Client& a)
    {
        return this->getHost() != a.getHost() || this->getPort() != a.getPort();
    }
};

