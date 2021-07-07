#include "NetClient.h"
#include <Windows.h>

enum class Protocal: uint32_t
{
	ACK = 0,
	IDLE,
	PING_SERVER,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT,
	MESSAGE_NUM
};

class Client : public NetClient<Protocal>
{
public:
	void Update() override
	{
		if (!m_messageIn.empty())
		{
			std::cout << "Message received" << std::endl;
			NetMessage<Protocal> msg = m_messageIn.pop_front();
			switch (msg.m_header.m_flag)
			{
			case Protocal::ACK:
				std::cout << "Connection is accepted. Your ID is " << std::endl;
				msg.Print();
				break;
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
				break;
			case Protocal::DISCONNECT:
				std::cout << "DISCONNECT" << std::endl;
				break;
			default:
				break;
			}
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
				client.m_messageOut.push_back(msgA);
				client.WriteMessage();
			}
			if (GetAsyncKeyState(0x42) < 0 && !Bpressed && !Apressed && !Cpressed && !Dpressed && !Epressed)
			{
				Bpressed = true;
				std::cout << "'B' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x42) == 0 && Bpressed)
			{
				Bpressed = false;
				client.m_messageOut.push_back(msgB);
				client.WriteMessage();
			}
			if (GetAsyncKeyState(0x43) < 0 && !Cpressed && !Bpressed && !Apressed && !Dpressed && !Epressed)
			{
				Cpressed = true;
				std::cout << "'C' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x43) == 0 && Cpressed)
			{
				Cpressed = false;
				client.m_messageOut.push_back(msgC);
				client.WriteMessage();
			}
			if (GetAsyncKeyState(0x44) < 0 && !Dpressed && !Bpressed && !Cpressed && !Apressed && !Epressed)
			{
				Dpressed = true;
				std::cout << "'D' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x44) == 0 && Dpressed)
			{
				Dpressed = false;
				client.m_messageOut.push_back(msgD);
				client.WriteMessage();
			}
			if (GetAsyncKeyState(0x45) < 0 && !Epressed && !Bpressed && !Cpressed && !Dpressed && !Apressed)
			{
				Epressed = true;
				std::cout << "'E' is pressed" << std::endl;
			}
			if (GetAsyncKeyState(0x45) == 0 && Epressed)
			{
				Epressed = false;
				client.m_messageOut.push_back(msgE);
				client.WriteMessage();
			}
		}
		client.Update();
	}
	return 0;
}