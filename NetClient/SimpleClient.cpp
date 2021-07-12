#include "NetClient.h"
#include <Windows.h>

enum class Protocal : uint32_t
{
	IDLE = 0,
	PING_SERVER,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT,
	MESSAGE_NUM
};

class Client : public NetClient<Protocal>
{
public:
	void Update()
	{
		if (!m_messageIn.empty())
		{
			std::cout << "Message received" << std::endl;
			NetMessage<Protocal> msg = m_messageIn.pop_front();
			switch (msg.m_header.m_flag)
			{
			case Protocal::IDLE:
				std::cout << "[Server]" << std::endl;
				msg.Print();
				std::cout << "IDLE" << std::endl;
				break;
			case Protocal::PING_SERVER:
				std::cout << "[Server]" << std::endl;
				msg.Print();
				std::cout << "PING_SERVER" << std::endl;
				break;
			case Protocal::GET_ID:
				std::cout << "[Server]" << std::endl;
				msg.Print();
				std::cout << "GET_ID" << std::endl;
				break;
			case Protocal::MESSAGE_ALL:
				std::cout << "Message from Client ID[" << msg.m_header.m_source_id << "]" << std::endl;
				msg.Print();
				std::cout << "MESSAGE_ALL" << std::endl;
				break;
			case Protocal::DISCONNECT:
				std::cout << "[Server]" << std::endl;
				msg.Print();
				std::cout << "DISCONNECT" << std::endl;
				break;
			default:
				break;
			}
			std::cout << std::endl;
		}
	}
};

int main()
{
	const HWND window2 = GetForegroundWindow();
	Client client;
	client.ConnectToServer();
	NetMessage<Protocal> msgA(client.m_uid, Protocal::IDLE, { 2,3,4,5,6 });
	NetMessage<Protocal> msgB(client.m_uid, Protocal::PING_SERVER, { 3,4,5,6,7 });
	NetMessage<Protocal> msgC(client.m_uid, Protocal::GET_ID, { 4,5,6,7,8 });
	NetMessage<Protocal> msgD(client.m_uid, Protocal::MESSAGE_ALL, { 5,6,7,8,9 });
	NetMessage<Protocal> msgE(client.m_uid, Protocal::DISCONNECT, { 6,7,8,9,10 });
	bool Apressed = false;
	bool Bpressed = false;
	bool Cpressed = false;
	bool Dpressed = false;
	bool Epressed = false;
	while (true)
	{
		if (window2 == GetForegroundWindow())
		{
			// hard coded get key state
			if (GetAsyncKeyState(0x41) < 0 && !Apressed && !Bpressed && !Cpressed && !Dpressed && !Epressed)
			{
				Apressed = true;
				std::cout << "'A' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x41) == 0 && Apressed)
			{
				Apressed = false;
				client.WriteMessage(msgA);
			}
			if (GetAsyncKeyState(0x42) < 0 && !Bpressed && !Apressed && !Cpressed && !Dpressed && !Epressed)
			{
				Bpressed = true;
				std::cout << "'B' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x42) == 0 && Bpressed)
			{
				Bpressed = false;
				client.WriteMessage(msgB);
			}
			if (GetAsyncKeyState(0x43) < 0 && !Cpressed && !Bpressed && !Apressed && !Dpressed && !Epressed)
			{
				Cpressed = true;
				std::cout << "'C' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x43) == 0 && Cpressed)
			{
				Cpressed = false;
				client.WriteMessage(msgC);
			}
			if (GetAsyncKeyState(0x44) < 0 && !Dpressed && !Bpressed && !Cpressed && !Apressed && !Epressed)
			{
				Dpressed = true;
				std::cout << "'D' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x44) == 0 && Dpressed)
			{
				Dpressed = false;
				client.WriteMessage(msgD);
			}
			if (GetAsyncKeyState(0x45) < 0 && !Epressed && !Bpressed && !Cpressed && !Dpressed && !Apressed)
			{
				Epressed = true;
				std::cout << "'E' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x45) == 0 && Epressed)
			{
				Epressed = false;
				client.WriteMessage(msgE);
			}
		}
		client.Update();
	}
	return 0;
}