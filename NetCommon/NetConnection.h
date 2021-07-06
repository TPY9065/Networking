#pragma once

#include "NetCommon.h"
#include "NetMessage.h"
#include "MessageQueue.h"

class NetConnection
{
public:
	enum class Owner
	{
		Server,
		Client
	};
public:
	NetConnection(Owner a_owner, asio::ip::tcp::socket a_socket, asio::io_context& a_context, uint32_t id, std::unordered_map<uint32_t, MessageQueue>& messageIn, MessageQueue& messageOut);
	~NetConnection();
	void ConnectToServer(asio::ip::tcp::endpoint& endpoint);
	void ConnectToClient();
	void ReadMessage();
	void ReadMessageHeader();
	void ReadMessageBody();
	void WriteMessage();
	void WriteMessageHeader();
	void WriteMessageBody();
	void Disconnect();
	bool IsAlive();
	Owner GetOwner();
	uint32_t GetId();
public:
	// id used for identification
	uint32_t m_uid;
	// owner of the connetion
	Owner m_owner;
	// socket for connection
	asio::ip::tcp::socket m_socket;
	// I/O context for asio
	asio::io_context& m_context;
	// buffer for message
	NetMessage m_message;
	// temperory message buffer
	NetMessage m_tempMsgBuf;
	// incoming/outgoing message queue
	std::unordered_map<uint32_t, MessageQueue>& m_messageIn;
	MessageQueue& m_messageOut;
};