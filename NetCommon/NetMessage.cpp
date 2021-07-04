#include "NetMessage.h"

NetMessageHeader::NetMessageHeader(uint32_t id)
{
	m_uid = id;
	m_size = 0;
}

NetMessageHeader::NetMessageHeader(uint32_t id, uint32_t size)
{
	m_uid = id;
	m_size = size;
}

NetMessage::NetMessage() : m_header(0), m_body({})
{

}

NetMessage::NetMessage(uint32_t id, std::vector<uint32_t> body): m_header(id, body.size()), m_body(body)
{

}

NetMessage::~NetMessage()
{

}

void NetMessage::Print()
{
	std::cout << "capacity: " << m_body.capacity() << std::endl;
	std::cout << "size: " << m_body.size() << std::endl;
	std::cout << "body: ";
	for (auto it = m_body.begin(); it != m_body.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void NetMessage::operator=(std::vector<uint32_t> body)
{
	m_body = body;
	m_header.m_size = body.size();
}

void NetMessage::operator=(NetMessage msg)
{
	m_header.m_uid = msg.m_header.m_uid;
	m_header.m_size = msg.m_header.m_size;
	m_body = msg.m_body;
}

NetMessage& NetMessage::operator<<(uint32_t data)
{
	m_body.push_back(data);
	//m_body.shrink_to_fit();
	m_header.m_size = m_body.size();
	return *this;
}

NetMessage& NetMessage::operator>>(uint32_t& data)
{
	assert(!m_body.empty());
	data = m_body.front();
	m_body.erase(m_body.begin());
	//m_body.shrink_to_fit();
	m_header.m_size = m_body.size();
	return *this;
}
