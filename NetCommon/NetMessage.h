#pragma once

#include "NetCommon.h"

class NetMessageHeader
{
public:
	NetMessageHeader(uint32_t id);
	NetMessageHeader(uint32_t id, uint32_t size);
public:
	// client id
	uint32_t m_uid;
	// body size in byte
	uint32_t m_size;
};

class NetMessage
{
public:
	NetMessage();
	NetMessage(uint32_t id, std::vector<uint32_t> body);
	~NetMessage();
	void Print();
	// only change the body of the message
	void operator=(std::vector<uint32_t> body);
	// copy the whole message into *this
	void operator=(NetMessage msg);
	NetMessage& operator<<(uint32_t data);
	NetMessage& operator>>(uint32_t& data);
public:
	// header for message
	NetMessageHeader m_header;
	// data to be sent
	std::vector<uint32_t> m_body;
};