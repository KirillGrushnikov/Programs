#pragma once
#include <iostream>

enum class SocketStatus : uint8_t {
	connected = 0,
	err_socket_init = 1,
	err_socket_bind = 2,
	err_socket_connect = 3,
	disconnected = 4
};

enum class SocketType : uint8_t {
	client_socket = 0,
	server_socket = 1
};

inline std::string strSocketStatus(SocketStatus status)
{
	std::string str;
	switch (status)
	{
	case SocketStatus::connected:
		str = "connected";
		break;
	case SocketStatus::err_socket_init:
		str = "error socket init";
		break;
	case SocketStatus::err_socket_bind:
		str = "error socket bind";
		break;
	case SocketStatus::err_socket_connect:
		str = "error socket connect";
		break;
	case SocketStatus::disconnected:
		str = "disconnected";
		break;
	}

	return str;
}

enum class DataType : uint8_t {
	message = 1,
	image = 2,
	image_size = 3
};

struct DataBuffer {
	int size = 0;
	void* data_ptr = nullptr;
	DataType type = DataType::message;

	DataBuffer() = default;
	DataBuffer(int size, void* data_ptr, DataType type) : size(size), data_ptr(data_ptr), type(type) {}
	DataBuffer(const DataBuffer& other) : size(other.size), data_ptr(malloc(size)), type(other.type) { memcpy(data_ptr, other.data_ptr, size); }
	~DataBuffer() { if (data_ptr) free(data_ptr); }

	bool isEmpty() const { return !data_ptr || !size; }
	operator bool() { return data_ptr && size; }
};

class ClientBase {
public:
	typedef SocketStatus status;
	virtual ~ClientBase() {};
	virtual SocketStatus disconnect() = 0;
	virtual bool sendData(const void* buffer, const size_t size, DataType data_type) const = 0;
	virtual DataBuffer loadData() = 0;
	virtual uint32_t getHost() const = 0;
	virtual uint16_t getPort() const = 0;
	virtual SocketStatus getStatus() const = 0;
	virtual SocketType getType() const = 0;
};