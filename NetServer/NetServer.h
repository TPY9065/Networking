#pragma once

#include "../NetCommon/NetConnection.h"
#include "../NetCommon/MessageQueue.h"

class NetServer
{
public:
	NetServer();
	~NetServer();
	void Start();
	void WaitForConnection();
	void ReadMessageFromClient(std::shared_ptr<NetConnection> connection);
	void WriteMessageToClient(std::shared_ptr<NetConnection> connection);
	void WriteMessageToAllClient(std::shared_ptr<NetConnection> from);
	void Disconnect(std::shared_ptr<NetConnection> connection);
	void Update();
public:
	// context for I/O services
	asio::io_context m_context;
	// acceptor for accepting connection
	asio::ip::tcp::acceptor m_acceptor;
	// thread for I/O
	std::thread m_thread;
	// container/manageer for keeping the newcoming connection alive
	std::deque<std::shared_ptr<NetConnection>> m_connections;
	// socket for sending message to client
	asio::ip::tcp::socket m_socket;
	// incoming message queue
	std::unordered_map<uint32_t, MessageQueue> m_messageIn;
	MessageQueue m_messageOut;
	// no of connections alived, also used for identification
	uint32_t m_uid = 10000;
};

