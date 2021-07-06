#include "NetClient.h"
#include <Windows.h>

enum class Protocal: uint32_t
{
	IDLE = 0,
	PING_SERVER,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT,
	MESSAGE_NUM
};

int main()
{
	const HWND window2 = GetForegroundWindow();
	NetClient<Protocal> client;
	client.ConnectToServer();
	NetMessage<Protocal> msgA(0, Protocal::IDLE, { 2,3,4,5,6 });
	NetMessage<Protocal> msgB(0, Protocal::PING_SERVER, { 3,4,5,6,7 });
	NetMessage<Protocal> msgC(0, Protocal::GET_ID, { 4,5,6,7,8 });
	NetMessage<Protocal> msgD(0, Protocal::MESSAGE_ALL, { 5,6,7,8,9 });
	NetMessage<Protocal> msgE(0, Protocal::DISCONNECT, { 6,7,8,9,10 });
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