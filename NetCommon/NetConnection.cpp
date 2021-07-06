#include "NetConnection.h"

NetConnection::NetConnection(NetConnection::Owner a_owner, asio::ip::tcp::socket a_socket, asio::io_context& a_context, uint32_t id, std::unordered_map<uint32_t, MessageQueue>& messageIn, MessageQueue& messageOut)
	: m_uid(id), m_socket(std::move(a_socket)), m_context(a_context), m_messageIn(messageIn), m_messageOut(messageOut), m_tempMsgBuf(), m_message(id, { 1,2,3 })
{
	m_owner = a_owner;
}

NetConnection::~NetConnection()
{
	if (m_socket.is_open())
	{
		Disconnect();
	}
	std::cout << "Client ID[" << m_uid << "]" << std::endl;
}

void NetConnection::ConnectToServer(asio::ip::tcp::endpoint& endpoint)
{
}

void NetConnection::ConnectToClient()
{
}

void NetConnection::ReadMessage()
{

}

void NetConnection::ReadMessageHeader()
{
	// read the message header, which contains the size of the body and id for identification, asynchronously
	m_socket.async_receive(asio::buffer(&m_tempMsgBuf.m_header, sizeof(NetMessageHeader)),
		[this](asio::error_code ec, size_t len)
		{
			if (!ec)
			{
				// if there is body in the message, resize the buffer size and ready to read body
				if (m_tempMsgBuf.m_header.m_size > 0)
				{
					m_tempMsgBuf.m_body.resize(m_tempMsgBuf.m_header.m_size);
					ReadMessageBody();
				}
				else
				// otherwise, back to read next message header
					ReadMessageHeader();
			}
			else
			{
				std::cout << "ReadMessageHeader(): " << ec.message() << std::endl;
				Disconnect();
			}
		});
}

void NetConnection::ReadMessageBody()
{
	// read the message body, which contains the main data, asynchronously
	m_socket.async_receive(asio::buffer(m_tempMsgBuf.m_body.data(), m_tempMsgBuf.m_header.m_size * sizeof(uint32_t)),
		[this](asio::error_code ec, size_t len)
		{
			if (!ec)
			{
				// push the newly received message into incoming message queue
				m_messageIn[m_uid].push_back(m_tempMsgBuf);
				// back to read next message header
				ReadMessageHeader();
			}
			else
			{
				std::cout << "ReadMessageBody(): " << ec.message() << std::endl;
				Disconnect();
			}
		});
}

void NetConnection::WriteMessage()
{
	// check if there is any waiting message for sending, if yes, ready to write message header
	if (!m_messageOut.empty())
	{
		m_message = m_messageOut.pop_front();
		m_message.m_header.m_uid = m_uid;
		WriteMessageHeader();
	}
}

void NetConnection::WriteMessageHeader()
{
	// write the message header, which contains the size of the body and id for identification, asynchronously
	m_socket.async_send(asio::buffer(&m_message.m_header, sizeof(NetMessageHeader)),
		[this](asio::error_code ec, size_t len)
		{
			// if no error occured during writing message header, ready for writing message body, otherwise, diconnect the connection
			if (!ec)
			{
				WriteMessageBody();
			}
			else
			{
				std::cout << "WriteMessageHeader(): " << ec.message() << std::endl;
				Disconnect();
			}
		}
	);
}

void NetConnection::WriteMessageBody()
{
	// write the message header, which contains the main data, asynchronously
	m_socket.async_send(asio::buffer(m_message.m_body.data(), m_message.m_header.m_size * sizeof(uint32_t)),
		[this](asio::error_code ec, size_t len)
		{
			// if no error occured during writing message body, back to write next message header, otherwise, diconnect the connection
			if (!ec)
			{
				WriteMessage();
			}
			else
			{
				std::cout << "WriteMessageBody(): " << ec.message() << std::endl;
				Disconnect();
			}
		}
	);
}

void NetConnection::Disconnect()
{
	if (m_socket.is_open())
		m_socket.close();
	if (m_owner == Owner::Server)
		std::cout << "[SERVER] Client ID[" << m_uid << "] is disconnected." << std::endl;
	else if (m_owner == Owner::Client)
		std::cout << "Disconnected from the server." << std::endl;
}

bool NetConnection::IsAlive()
{
	return m_socket.is_open();
}

NetConnection::Owner NetConnection::GetOwner()
{
	return m_owner;
}

uint32_t NetConnection::GetId()
{
	return m_uid;
}