#pragma once

#include <Networking.h>

class NetClient
{
public:
	NetClient();
	~NetClient();
	void Disconnect();
	void ConnectToServer();
	void ReadMessage();
	void WriteMessage();
	void Update();
	bool IsConnected();
public:
	// context to write message by using asio
	asio::io_context m_context;
	// thread for io
	std::thread m_thread;
	// socket for connection
	asio::ip::tcp::socket m_socket;
	// id
	uint32_t m_uid;
	// connection
	std::unique_ptr<NetConnection> m_connection;
	// incoming message queue
	MessageQueue m_messageIn;
	// outgoing message queue
	MessageQueue m_messageOut;
};

