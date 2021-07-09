#include "NetServer.h"

enum class Protocal : uint32_t
{
	IDLE = 0,
	PING_SERVER,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT,
	MESSAGE_NUM
};

class Server : public NetServer<Protocal>
{
public:
	void Update()
	{
		if (!m_messageIn.empty())
		{
			NetMessage<Protocal> msg = m_messageIn.pop_front();
			std::cout << "Client ID[" << msg.m_header.m_source_id << "]: " << std::endl;
			msg.Print();
			switch (msg.m_header.m_flag)
			{
			case Protocal::IDLE:
				std::cout << "IDLE" << std::endl;
				break;
			case Protocal::PING_SERVER:
				std::cout << "PING_SERVER" << std::endl;
				break;
			case Protocal::GET_ID:
				std::cout << "GET_ID" << std::endl;
				break;
			case Protocal::MESSAGE_ALL:
				std::cout << "MESSAGE_ALL" << std::endl;
				MessageToAllClient(msg, msg.m_header.m_source_id);
				break;
			case Protocal::DISCONNECT:
				std::cout << "DISCONNECT" << std::endl;
				break;
			default:
				break;
			}
			std::cout << std::endl;
		}
	}
	void Run()
	{
		while (true)
			Update();
	}
};

int main()
{
	// construct a server obj
	Server server;
	// start the server
	server.Start();
	// run the server
	server.Run();
	return 0;
}