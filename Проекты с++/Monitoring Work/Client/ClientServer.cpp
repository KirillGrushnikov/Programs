#include "ClientServer.h"

std::vector<char> ClientServer::printScreen()
{
    // ��������� �������� ����������� ������� �������
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, x, y, SRCCOPY);
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
    DeleteDC(hMemoryDC);
    DeleteDC(hScreenDC);

    BITMAP bitmapInfo;
    GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);

    BITMAPINFOHEADER bih = { 0 };
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 0; // ����� ���������� � 0 ��� BI_RGB
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    BITMAPINFO bi;
    bi.bmiHeader = bih;

    // ������ ������� � pixelData
    std::vector<char> pixelData(bitmapInfo.bmWidthBytes * bitmapInfo.bmHeight);
    HDC hdc = GetDC(NULL);
    GetDIBits(hdc, hBitmap, 0, bitmapInfo.bmHeight, &pixelData[0], &bi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);

    DeleteObject(hBitmap);
    image_size_data = std::to_string(bitmapInfo.bmWidth).c_str();
    image_size_data += ' ';
    image_size_data += std::to_string(bitmapInfo.bmHeight).c_str();

    return pixelData;
}

void ClientServer::handlerData(DataBuffer data, Client& client)
{
    std::string data_str = dataToStr(data);
    std::cout << "\nMessage received:\n";
    std::cout << "ip: " << ipToStr(client.getHost()) << ":" << ntohs(client.getPort()) << std::endl;
    std::cout << data_str;

    if (data_str == "ps")
    {
        std::vector<char> image_data = printScreen();
        send_data_type = DataType::image_size;
        sendDataBy(client.getHost(), client.getPort(), image_size_data.c_str(), image_size_data.size());
        send_data_type = DataType::image;
        sendDataBy(client.getHost(), client.getPort(), &image_data[0], image_data.size());
    }
    else
    {
        send_data_type = DataType::message;
        sendDataBy(client.getHost(), client.getPort(), data_str.c_str(), data_str.size());
    }
}

void ClientServer::handlerConnect(Client& client)
{
    std::cout << "\nServer connect:\n";
    std::cout << "ip: " << ipToStr(client.getHost()) << ":" << ntohs(client.getPort()) << std::endl;
}

void ClientServer::handlerDisconnect(Client& client)
{
    using namespace std::chrono_literals;
    std::cout << "\nServer disconnect:\n";
    std::cout << "ip: " << ipToStr(client.getHost()) << ":" << ntohs(client.getPort()) << std::endl;

    std::this_thread::sleep_for(20000ms);
    uint32_t host; inet_pton(AF_INET, "127.0.0.1", &host);
    while (!connectToServer(host, 1234))
    {
        std::this_thread::sleep_for(500ms);
        std::cout << "connected failed\r";
    }
}

bool ClientServer::sendDataBy(uint32_t host, uint16_t port, const void* buffer, const size_t size)
{
    bool data_is_sended = false;
    for (std::unique_ptr<Client>& client : client_list)
        if (client->getHost() == host && client->getPort() == port) {
            client->sendData(buffer, size, send_data_type);
            data_is_sended = true;
        }
    return data_is_sended;
}

ClientServer::ClientServer(const uint16_t port) : NetworkHandler(port)
{
}