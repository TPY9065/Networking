#ifndef _NETCLIENT_CPP_
#define _NETCLIENT_CPP_
#include "NetClient.h"

template<typename T>
NetClient<T>::NetClient() : m_socket(m_context)
{	
	m_uid = 0;
}

template<typename T>
NetClient<T>::~NetClient()
{
	Disconnect();
	m_context.stop();
}

template<typename T>
void NetClient<T>::ConnectToServer()
{
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
				m_connection = std::make_unique<NetConnection<T>>(NetConnection<T>::Owner::Client, std::move(m_socket), m_context, 0, m_messageIn, m_messageOut);
				
				// start receiving message
				ReadMessage();
			}
			else
				std::cout << "ConnectToServer(): " << ec.message() << std::endl;
		}
	);

	m_thread = std::thread([this]() {m_context.run(); });
}

template<typename T>
void NetClient<T>::ReadMessage()
{
	// check if the connection is still alive
	if (m_connection->IsAlive())
		// if yes, ready to read message
		m_connection->ReadMessageHeader();
	else
		// otherwise, disconnect to the server
		Disconnect();
}

template<typename T>
void NetClient<T>::WriteMessage()
{
	// check if the connection is still alive
	if (m_connection->IsAlive())
		// if yes, ready to write message
		m_connection->WriteMessage();
	else
	{
		std::cout << "Disconnected from server." << std::endl;
		// otherwise, disconnect to the server
		// Disconnect();
	}
}

template<typename T>
void NetClient<T>::Disconnect()
{
	// check if the current connection is alive, if yes, disconnected it, otherwise, do nothing
	if (m_connection->IsAlive())
	{
		m_connection->Disconnect();
		m_connection.reset();
	}
}

template<typename T>
void NetClient<T>::Update()
{
	// check if there is any message received, if yes, pop from the message queue and print it on screen
	if (!m_messageIn.empty())
	{
		std::cout << "Server: " << std::endl;
		m_messageIn.pop_front().Print();
	}
}

template<typename CustomMessage>
uint32_t NetClient<CustomMessage>::Hash(uint32_t plaintext)
{
	return (11 * plaintext + 4) % 26;
}

template<typename T>
bool NetClient<T>::IsConnected()
{
	return m_connection->IsAlive();
}

#endif