#include "NetMessage.h"

template<typename T>
NetMessageHeader<T>::NetMessageHeader(uint32_t id)
{
	m_uid = id;
	m_type = 0;
	m_size = 0;
}

template<typename T>
NetMessageHeader<T>::NetMessageHeader(uint32_t id, T type, uint32_t size)
{
	m_uid = id;
	m_type = type;
	m_size = size;
}

template<typename CustomMessage>
NetMessage<CustomMessage>::NetMessage() : m_header(0), m_body({})
{

}

template<typename CustomMessage>
NetMessage<CustomMessage>::NetMessage(uint32_t id, CustomMessage type, std::vector<uint32_t> body): m_header(id, type, body.size()), m_body(body)
{

}

template<typename CustomMessage>
NetMessage<CustomMessage>::~NetMessage()
{

}

template<typename CustomMessage>
void NetMessage<CustomMessage>::Print()
{
	std::cout << "capacity: " << m_body.capacity() << std::endl;
	std::cout << "size: " << m_body.size() << std::endl;
	std::cout << "body: ";
	for (auto it = m_body.begin(); it != m_body.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

template<typename CustomMessage>
void NetMessage<CustomMessage>::operator=(std::vector<uint32_t> body)
{
	m_body = body;
	m_header.m_size = body.size();
}

template<typename CustomMessage>
void NetMessage<CustomMessage>::operator=(NetMessage msg)
{
	m_header.m_uid = msg.m_header.m_uid;
	m_header.m_size = msg.m_header.m_size;
	m_body = msg.m_body;
}

template<typename CustomMessage>
NetMessage<CustomMessage>& NetMessage<CustomMessage>::operator<<(uint32_t data)
{
	m_body.push_back(data);
	//m_body.shrink_to_fit();
	m_header.m_size = m_body.size();
	return *this;
}

template<typename CustomMessage>
NetMessage<CustomMessage>& NetMessage<CustomMessage>::operator>>(uint32_t& data)
{
	assert(!m_body.empty());
	data = m_body.front();
	m_body.erase(m_body.begin());
	//m_body.shrink_to_fit();
	m_header.m_size = m_body.size();
	return *this;
}
