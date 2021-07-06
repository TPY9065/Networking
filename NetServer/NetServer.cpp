#ifndef _NETSERVER_CPP_
#define _NETSERVER_CPP_
#include "NetServer.h"

// initialize an acceptor and open a tcp socket at port
template<typename CustomMessage>
NetServer<CustomMessage>::NetServer() : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 60000)), m_socket(m_context), m_messageIn()
{
	m_uid = 10000;
}

template<typename CustomMessage>
NetServer<CustomMessage>::~NetServer()
{
	// stop the context
	m_context.stop();
}

// start the server
template<typename CustomMessage>
void NetServer<CustomMessage>::Start()
{
	// start thread for io
	m_thread = std::thread([this]() { m_context.run(); });

	std::cout << "[SERVER] Started!\n";

	// start waiting connection
	WaitForConnection();
}

template<typename CustomMessage>
void NetServer<CustomMessage>::WaitForConnection()
{
	// creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	m_acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
				// keep the new connection alive
				std::shared_ptr<NetConnection<CustomMessage>> newConnecion = std::make_shared<NetConnection<CustomMessage>>(NetConnection<CustomMessage>::Owner::Server, std::move(socket), m_context, m_uid, m_messageIn, m_messageOut);
				m_connections.push_back(std::move(newConnecion));

				// send message to inform client that the connection is accepted
				// WriteMessageToClient(m_connections.back());
				// start reading message from clients
				ReadMessageFromClient(m_connections.back());

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

template<typename CustomMessage>
void NetServer<CustomMessage>::ReadMessageFromClient(std::shared_ptr<NetConnection<CustomMessage>> connection)
{
	// safe guard, prevent client disconnected instantly after connecting to server
	if (connection->IsAlive())
		connection->ReadMessageHeader();
	else
		Disconnect(connection);
}

template<typename CustomMessage>
void NetServer<CustomMessage>::WriteMessageToClient(std::shared_ptr<NetConnection<CustomMessage>> connection)
{
	if (connection->IsAlive())
		connection->WriteMessageHeader();
	else
		Disconnect(connection);
}

template<typename CustomMessage>
void NetServer<CustomMessage>::WriteMessageToAllClient(std::shared_ptr<NetConnection<CustomMessage>> from)
{
	// write message to all client, except the one who send message
	std::deque<std::shared_ptr<NetConnection>> disconnectedConnections;
	for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++)
		if (*connection != from && (*connection)->IsAlive())
			(*connection)->WriteMessageHeader();
		else
			disconnectedConnections.push_back((*connection));

	for (auto disconnectedConnection = disconnectedConnections.begin(); disconnectedConnection != disconnectedConnections.end(); disconnectedConnection++)
		Disconnect((*disconnectedConnection));
}

template<typename CustomMessage>
void NetServer<CustomMessage>::Disconnect(std::shared_ptr<NetConnection<CustomMessage>> connection)
{
	// remove the pointer from the container/manager first, otherwise remove will not work after reset
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection), m_connections.end());
	// Destroy the objcet point by the pointer
	connection.reset();
}

template<typename CustomMessage>
void NetServer<CustomMessage>::Update()
{
	// check if there is any message sent from the client, if yes, pop out from the message queue and print it on screen
	for (auto client = m_messageIn.begin(); client != m_messageIn.end(); client++)
		if (!client->second.empty())
		{
			std::cout << "Client ID[" << client->first << "]: " << std::endl;
			NetMessage<CustomMessage> msg = client->second.pop_front();
			msg.Print();
		}
}

#endif // !_NETSERVER_CPP_