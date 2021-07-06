#include "NetClient.h"

NetClient::NetClient() : m_socket(m_context), m_uid(0)
{	

}

NetClient::~NetClient()
{

}

void NetClient::ConnectToServer()
{
	m_thread = std::thread([this]() {m_context.run(); });
	// define the endpoint to be connected
	asio::ip::address address = asio::ip::make_address("127.0.0.1");
	asio::ip::tcp::endpoint endpoint(address, 60000);
	// establish a socket connection to remote endpoint
	m_socket.async_connect(endpoint, 
		[this](std::error_code ec)
		{
			if (!ec)
			{
				// keep the connection alive
				m_connection = std::make_unique<NetConnection>(NetConnection::Owner::Client, std::move(m_socket), m_context, 0, m_messageIn, m_messageOut);
				ReadMessage();
			}
			else
				std::cout << "ConnectToServer(): " << ec.message() << std::endl;
		}
	);
}

void NetClient::ReadMessage()
{
	// check if the connection is still alive
	if (m_connection->IsAlive())
		// if yes, ready to read message
		m_connection->ReadMessageHeader();
	else
		// otherwise, disconnect to the server
		Disconnect();
}

void NetClient::WriteMessage()
{
	// check if the connection is still alive
	if (m_connection->IsAlive())
		// if yes, ready to write message
		m_connection->WriteMessage();
	else 
		// otherwise, disconnect to the server
		Disconnect();
}

void NetClient::Disconnect()
{
	// check if the current connection is alive, if yes, disconnected it, otherwise, do nothing
	if (m_connection->IsAlive())
	{
		m_connection->Disconnect();
		m_connection.reset();
	}
}

void NetClient::Update()
{
	// check if there is any message sent from the server, if yes, pop from the message queue and print it on screen
	if (!m_messageIn.empty())
	{
		std::cout << "Server: " << std::endl;
		m_messageIn[m_uid].pop_front().Print();
	}
}

bool NetClient::IsConnected()
{
	return m_connection->IsAlive();
}