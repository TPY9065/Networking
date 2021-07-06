#pragma once

#include "NetCommon.h"
#include "NetMessage.h"

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();
	void push_back(NetMessage msg);
	void push_front(NetMessage msg);
	NetMessage pop_back();
	NetMessage pop_front();
	NetMessage& back();
	NetMessage& front();
	bool empty();
	uint32_t size();
public:
	// message to that socket
	std::deque<NetMessage> m_messageQueue;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};