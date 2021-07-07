#include "NetServer.h"

enum class Protocal : uint32_t
{
	ACK = 0,
	IDLE,
	PING_SERVER,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT,
	MESSAGE_NUM
};

class Server : public NetServer<Protocal>
{
public:
	void Update() override
	{
		if (!m_messageIn.empty())
		{
			NetMessage<Protocal> msg = m_messageIn.pop_front();
			std::cout << "Client ID[" << msg.m_header.m_source_id << "]: " << std::endl;
			msg.Print();
			/*
			switch (msg.m_header.m_flag)
			{
			case Protocal::IDLE:
				std::cout << "IDLE" << std::endl;
				//MessageToClient(msg, msg.m_header.m_uid);
				break;
			case Protocal::PING_SERVER:
				std::cout << "PING_SERVER" << std::endl;
				break;
			case Protocal::GET_ID:
				std::cout << "GET_ID" << std::endl;
				break;
			case Protocal::MESSAGE_ALL:
				std::cout << "MESSAGE_ALL" << std::endl;
				break;
			case Protocal::DISCONNECT:
				std::cout << "DISCONNECT" << std::endl;
				break;
			default:
				break;
			}
			*/
		}
	}
};

int main()
{
	// construct a server obj
	Server server;
	// start the server
	server.Start();
	// start listening
	while (true)
	{
		server.Update();
	}
	return 0;
}