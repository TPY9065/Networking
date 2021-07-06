#include "MessageQueue.h"

MessageQueue::MessageQueue() :m_messageQueue({})
{
}

MessageQueue::~MessageQueue()
{
}

void MessageQueue::push_back(NetMessage msg)
{
	m_mutex.lock();
	m_messageQueue.push_back(msg);
	m_mutex.unlock();
}

void MessageQueue::push_front(NetMessage msg)
{
	m_mutex.lock();
	m_messageQueue.push_front(msg);
	m_mutex.unlock();
}

NetMessage MessageQueue::pop_back()
{
	m_mutex.lock();
	NetMessage msg = m_messageQueue.back();
	m_messageQueue.pop_back();
	m_mutex.unlock();
	return msg;
}

NetMessage MessageQueue::pop_front()
{
	m_mutex.lock();
	NetMessage msg = m_messageQueue.front();
	m_messageQueue.pop_front();
	m_mutex.unlock();
	return msg;
}

NetMessage& MessageQueue::back()
{
	return m_messageQueue.back();
}

NetMessage& MessageQueue::front()
{
	return m_messageQueue.front();
}

bool MessageQueue::empty()
{
	return m_messageQueue.empty();
}

uint32_t MessageQueue::size()
{
	return m_messageQueue.size();
}