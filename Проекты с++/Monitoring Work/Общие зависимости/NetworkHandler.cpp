#include "NetworkHandler.h"

NetworkHandler* NetworkHandler::instance = nullptr;

BOOL __stdcall NetworkHandler::ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_CLOSE_EVENT) {
        instance->stop();
        instance = nullptr;
    }
    return TRUE;
}

std::string NetworkHandler::dataToStr(const DataBuffer& data)
{
    std::string data_str;
    char* ptr = static_cast<char*>(data.data_ptr);
    for (int i = 0; i < data.size; ++i)
        data_str += ptr[i];

    data_str.erase(data_str.begin());
    return data_str;
}

std::string NetworkHandler::ipToStr(uint32_t host)
{
    uint32_t ip = htonl(host);
    ip = ntohl(ip);
    char ip_str[INET_ADDRSTRLEN];
    std::string ip_string = inet_ntop(AF_INET, &ip, ip_str, INET_ADDRSTRLEN);

    return ip_string;
}

void NetworkHandler::handlerData(DataBuffer& data, Client& client)
{
    std::string data_str = dataToStr(data);
    std::cout << "\nMessage received:\n";
    std::cout << "ip: " << ipToStr(client.getHost())  << std::endl;
    std::cout << data_str;
}

void NetworkHandler::handlerConnect(Client& client)
{
    std::cout << "\nClient connect:\n";
    std::cout << "ip: " << ipToStr(client.getHost()) << std::endl;
    sendData("", 0);
}

void NetworkHandler::handlerDisconnect(Client& client)
{
    std::cout << "\nClient disconnect:\n";
    std::cout << "ip: " << ipToStr(client.getHost()) << ":" << client.getPort() << std::endl;

}

bool NetworkHandler::connectToServer(uint32_t host, uint16_t port)
{
    int client_socket;
    sockaddr_in address;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (client_socket == INVALID_SOCKET) 
        return false;

    new(&address) sockaddr_in;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = host;
    address.sin_addr.S_un.S_addr = host;
    address.sin_port = htons(port);


    int is_connect = connect(client_socket, (sockaddr*)&address, sizeof(address));
    if (is_connect < 0)
        return false;

    std::unique_ptr<Client> client(new Client(client_socket, address));

    handlerConnect(*client);

    client_mutex.lock();
    client_list.emplace_back(std::move(client));
    client_mutex.unlock();

    return true;
}

ServerStatus NetworkHandler::start()
{
    if (_status == ServerStatus::up) stop();

    if (WSAStartup(MAKEWORD(2, 2), &w_data) != 0) 
        return _status = ServerStatus::err_socket_init;

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_addr.S_un.S_addr = INADDR_ANY;
    address.sin_port = htons(port);

    serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (serverSocket < 0)
        return _status = ServerStatus::err_socket_init;

    int is_bind = bind(serverSocket, (struct sockaddr*)&address, sizeof(address));
    if(is_bind < 0)
        return _status = ServerStatus::err_socket_bind;

    if (listen(serverSocket, SOMAXCONN))
        return _status = ServerStatus::err_socket_listening;

    _status = ServerStatus::up;

    newConnectionHandler_thread = std::thread([this] {handlerNewConnection(); });
    waitingData_thread = std::thread([this] {waitingData(); });

    return _status;
}

void NetworkHandler::stop()
{
    _status = ServerStatus::close;
    disconnectAll();
    shutdown(serverSocket, SD_BOTH);
    closesocket(serverSocket);
    WSACleanup();
}

ServerStatus NetworkHandler::getStatus() const
{
    return _status;
}

std::string NetworkHandler::getStatusStr() const
{
    std::string str;
    switch (_status)
    {
    case ServerStatus::up:
        str = "server active";
        break;
    case ServerStatus::err_socket_init:
        str = "error socket init";
        break;
    case ServerStatus::err_socket_bind:
        str = "error socket bind";
        break;
    case ServerStatus::err_socket_listening:
        str = "error socket listening";
        break;
    case ServerStatus::close:
        str = "server close";
        break;
    }
    return str;
}

void NetworkHandler::addUser(Client& client)
{
    int sch = 0;
    for (int i = 0; i < client_all.size(); ++i)
    {
        if (client.address.sin_addr.S_un.S_addr == client_all[i].address.sin_addr.S_un.S_addr && client.address.sin_port == client_all[i].address.sin_port)
        {
            client_all[i].updateLastTimeActive();
            break;
        }
        sch += 1;
    }
    if (sch == client_all.size())
    {
        ClientData c(client.address);
        c.user_name = client.user_name;
        c.computer_name = client.computer_name;
        c.work_group = client.work_group;
        c.updateLastTimeActive();
        client_all.push_back(c);
    }
}

void NetworkHandler::waitingData()
{
    using namespace std::chrono_literals;

    while (_status == ServerStatus::up)
    {
        bool is_disconnect = false;
        Client* pointer = nullptr;
        client_mutex.lock();
        for (auto it = client_list.begin(); it != client_list.end(); ++it) {
            auto& client = *it;
            if (client->_status == SocketStatus::connected) {
                DataBuffer data = client->loadData();
                if (!data.isEmpty()) {
                    handlerData(data, *client);
                }
            }
            else if (client->_status == SocketStatus::disconnected)
            {
                pointer = client.release();
                client_list.erase(it);
                is_disconnect = true;
                break;
            }
        }
        
        if (is_disconnect)
        {
            handlerDisconnect(*pointer);
            delete pointer;
        }
        client_mutex.unlock();

        std::this_thread::sleep_for(50ms);
    }
}

void NetworkHandler::handlerNewConnection()
{
    int addrlen = sizeof(sockaddr_in);
    while (_status == ServerStatus::up)
    {
        // новое подключение
        sockaddr_in clientAddres;
        int clientSocet = accept(serverSocket, (sockaddr*)&clientAddres, &addrlen);
        if (clientSocet != 0 && _status == ServerStatus::up)
        {
            getpeername(clientSocet, (sockaddr*)&clientAddres, &addrlen);

            std::unique_ptr<Client> client(new Client(clientSocet, clientAddres));

            handlerConnect(*client);

            client_mutex.lock();
            addUser(*client);
            client_list.emplace_back(std::move(client));
            client_mutex.unlock();
        }
    }
}

void NetworkHandler::joinTheads()
{
    newConnectionHandler_thread.join(); 
    waitingData_thread.join();
}

void NetworkHandler::sendData(const void* buffer, const size_t size)
{
    for (std::unique_ptr<Client>& client : client_list)
        client->sendData(buffer, size, DataType::message);
}

bool NetworkHandler::sendDataBy(uint32_t host, uint16_t port, const void* buffer, const size_t size)
{
    bool data_is_sended = false;
    for (std::unique_ptr<Client>& client : client_list)
        if (client->getHost() == host && client->getPort() == port) {
            client->sendData(buffer, size, DataType::message);
            data_is_sended = true;
        }
    return data_is_sended;
}

bool NetworkHandler::disconnectBy(uint32_t host, uint16_t port)
{
    bool client_is_disconnected = false;
    for (std::unique_ptr<Client>& client : client_list)
        if (client->getHost() == host && client->getPort() == port) {
            client->disconnect();
            client_is_disconnected = true;
        }
    return client_is_disconnected;
}

void NetworkHandler::disconnectAll()
{
    for (std::unique_ptr<Client>& client : client_list)
        client->disconnect();
}

NetworkHandler::NetworkHandler(const uint16_t port) : 
    port(port)
{
    instance = this;
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
}

NetworkHandler::~NetworkHandler()
{
    if (_status == ServerStatus::up)
        stop();
}




NetworkHandler::Client::Client(int socket, sockaddr_in address) :
    socket(socket),
    address(address)
{

}

NetworkHandler::Client::~Client()
{
    shutdown(socket, SD_BOTH);
    closesocket(socket);
}

uint32_t NetworkHandler::Client::getHost() const
{
    return address.sin_addr.S_un.S_addr;
}

uint16_t NetworkHandler::Client::getPort() const
{
    return address.sin_port;
}

SocketStatus NetworkHandler::Client::disconnect()
{
    _status = SocketStatus::disconnected;
    shutdown(socket, SD_BOTH);
    closesocket(socket);
    return _status;
}

DataBuffer NetworkHandler::Client::loadData()
{
    DataBuffer buffer;

    if(_status != SocketStatus::connected) return DataBuffer();
    unsigned long mode = 1; // 1 для включения неблокирующего режима
    ioctlsocket(socket, FIONBIO, &mode);

    int answ = recv(socket, (char*)&buffer.size, sizeof(buffer.size), 0);
    if (!answ) {
        disconnect();
        return DataBuffer();
    }

    mode = 0;
    ioctlsocket(socket, FIONBIO, &mode);

    if (!buffer.size) return DataBuffer();
    buffer.data_ptr = (char*)malloc(buffer.size);
    if(!buffer.data_ptr) return DataBuffer();
    int totalBytes = 0;
    int bytesRead = 0;
    int bites = buffer.size;
    while (totalBytes != buffer.size)
    {
        bytesRead = recv(socket, (char*)buffer.data_ptr + totalBytes, bites, 0);
        totalBytes += bytesRead;
        bites -= bytesRead;
    }

    uint8_t* new_buffer = static_cast<uint8_t*>(buffer.data_ptr);
    buffer.type = static_cast<DataType>(new_buffer[0]);

    return buffer;
}

bool NetworkHandler::Client::sendData(const void* buffer, const size_t size, DataType data_type) const
{
    if (_status != SocketStatus::connected) 
        return false;

    void* send_buffer = malloc(size + sizeof(DataType) + sizeof(int));
    *(int*)send_buffer = size + sizeof(DataType);
    memcpy((char*)send_buffer + sizeof(int), &data_type, sizeof(DataType));
    memcpy((char*)send_buffer + sizeof(DataType) + sizeof(int), buffer, size);

    int is_send = send(socket, (char*)send_buffer, size + sizeof(DataType) + sizeof(int), 0);
    if (is_send < 0)
        return false;

    free(send_buffer);
    return true;
}

std::string NetworkHandler::ClientData::getLastTimeActive()
{
    char date_time_str[100];
    std::strftime(date_time_str, sizeof(date_time_str), "%Y.%m.%d %H:%M", last_active_time);
    return std::string(date_time_str);
}

void NetworkHandler::ClientData::updateLastTimeActive()
{
    time_t now = time(0);
    last_active_time = new tm;
    localtime_s(last_active_time, &now);
}

NetworkHandler::ClientData::ClientData(sockaddr_in address) : address(address)
{
}
