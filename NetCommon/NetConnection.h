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
	NetConnection(Owner a_owner, asio::ip::tcp::socket a_socket, asio::io_context& a_context, uint16_t id, MessageQueue& messageIn, MessageQueue& messageOut);
	~NetConnection();
	void ConnectToServer(asio::ip::tcp::endpoint& endpoint);
	void ConnectToClient();
	void ReadMessageHeader();
	void ReadMessageBody();
	void WriteMessage();
	void WriteMessageHeader();
	void WriteMessageBody();
	void Disconnect();
	bool IsAlive();
	Owner GetOwner();
	uint16_t GetId();
public:
	// id used for identification
	uint16_t m_id;
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
	MessageQueue& m_messageIn;
	MessageQueue& m_messageOut;
};