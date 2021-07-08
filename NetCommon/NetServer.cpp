#ifndef _NETSERVER_CPP_
#define _NETSERVER_CPP_
#include "NetServer.h"

// initialize an acceptor and open a tcp socket at port
template<typename T>
NetServer<T>::NetServer() : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 60000)), m_socket(m_context), m_messageIn()
{
	m_uid = 10000;
}

template<typename T>
NetServer<T>::~NetServer()
{
	// stop the context
	m_context.stop();
}

// start the server
template<typename T>
void NetServer<T>::Start()
{
	// start thread for io
	m_thread = std::thread([this]() { m_context.run(); });

	std::cout << "[SERVER] Started!\n";

	// start waiting connection
	WaitForConnection();
}

template<typename T>
void NetServer<T>::WaitForConnection()
{
	// creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	m_acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
				// keep the new connection alive
				std::shared_ptr<NetConnection<T>> newConnecion = std::make_shared<NetConnection<T>>(NetConnection<T>::Owner::Server, std::move(socket), m_context, m_uid, m_messageIn, m_messageOut);
				m_connections[m_uid] = std::move(newConnecion);

				// start reading message from clients
				ReadMessageFromClient(m_connections[m_uid]);

				// assign id to the connection
				NetMessage<T> msg(0, static_cast<T>(HandShake::ACK), { m_uid });
				MessageToClient(msg, m_uid);

				// id for next connection + 1
				m_uid += 1;
			}
			else
				std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";

			// wait for another connection
			WaitForConnection();
		}
	);
}

template<typename T>
void NetServer<T>::ReadMessageFromClient(std::shared_ptr<NetConnection<T>> connection)
{
	// safe guard, prevent client disconnected instantly after connecting to server
	if (connection->IsAlive())
		connection->ReadMessageHeader();
	else
		Disconnect(connection);
}

template<typename T>
void NetServer<T>::MessageToClient(NetMessage<T> msg, uint32_t id)
{
	if (m_connections[id]->IsAlive())
	{
		m_connections[id]->m_messageOut.push_back(msg);
		m_connections[id]->WriteMessage();
	}
	else
		Disconnect(m_connections[id]);
}

template<typename T>
void NetServer<T>::MessageToAllClient(NetMessage<T> msg, std::shared_ptr<NetConnection<T>> from)
{
	// write message to all client, except the one who send message
	std::deque<std::shared_ptr<NetConnection>> disconnectedConnections;
	for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++)
		if (connection->second != from && connection->second->IsAlive())
			connection->second->WriteMessageHeader();
		else
			disconnectedConnections.push_back(connection->second);

	for (auto disconnectedConnection = disconnectedConnections.begin(); disconnectedConnection != disconnectedConnections.end(); disconnectedConnection++)
		Disconnect((*disconnectedConnection));
}

template<typename T>
void NetServer<T>::Disconnect(std::shared_ptr<NetConnection<T>> connection)
{
	// remove the pointer from the container/manager first, otherwise remove will not work after reset
	m_connections[connection->m_uid].reset();
	// Destroy the objcet point by the pointer
	connection.reset();
}

template<typename T>
void NetServer<T>::Update()
{
	// check if there is any message sent from the client, if yes, pop out from the message queue and print it on screen
	if (!m_messageIn.empty())
	{
		m_messageIn.pop_front().Print();
	}
}

#endif // !_NETSERVER_CPP_