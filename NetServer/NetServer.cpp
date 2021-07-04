#include "NetServer.h"

// initialize an acceptor and open a tcp socket at port
NetServer::NetServer() : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 60000)), m_socket(m_context), m_messageIn()
{
	m_uid = 10000;
}

NetServer::~NetServer()
{
	// stop the context
	m_context.stop();
}

// start the server
void NetServer::Start()
{
	// start thread for io
	m_thread = std::thread([this]() { m_context.run(); });

	std::cout << "[SERVER] Started!\n";

	// start waiting connection
	WaitForConnection();
}

void NetServer::WaitForConnection()
{
	// creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	m_acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
				// keep the new connection alive
				std::shared_ptr<NetConnection> newConnecion = std::make_shared<NetConnection>(NetConnection::Owner::Server, std::move(socket), m_context, m_uid, m_messageIn, m_messageOut);
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

void NetServer::ReadMessageFromClient(std::shared_ptr<NetConnection> connection)
{
	// safe guard, prevent client disconnected instantly after connecting to server
	if (connection->IsAlive())
		connection->ReadMessageHeader();
	else
		Disconnect(connection);
}

void NetServer::WriteMessageToClient(std::shared_ptr<NetConnection> connection)
{
	if (connection->IsAlive())
		connection->WriteMessageHeader();
	else
		Disconnect(connection);
}

void NetServer::WriteMessageToAllClient(std::shared_ptr<NetConnection> from)
{
	// write message to all client, except the one who send message
	std::deque<std::shared_ptr<NetConnection>> disconnectedConnections;
	for (int i = 0; i < m_connections.size(); i++)
		if (m_connections[i] != from && m_connections[i]->IsAlive())
			m_connections[i]->WriteMessageHeader();
		else
			disconnectedConnections.push_back(m_connections[i]);

	for (int i = 0; i < disconnectedConnections.size(); i++)
		Disconnect(disconnectedConnections[i]);
}

void NetServer::Disconnect(std::shared_ptr<NetConnection> connection)
{
	// remove the pointer from the container/manager first, otherwise remove will not work after reset
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection), m_connections.end());
	// Destroy the objcet point by the pointer
	connection.reset();
}

void NetServer::Update()
{
	// check if there is any message sent from the server, if yes, pop from the message queue and print it on screen
	if (!m_messageIn.empty())
	{
		NetMessage msg = m_messageIn.pop_front();
		msg.Print();
	}
}