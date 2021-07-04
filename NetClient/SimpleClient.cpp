#include "NetClient.h"
#include <Windows.h>

int main()
{
	NetClient client;
	client.ConnectToServer();
	NetMessage msgA(0, { 2,3,4,5,6 });
	NetMessage msgB(0, { 3,4,5,6,7 });
	NetMessage msgC(0, { 4,5,6,7,8 });
	NetMessage msgD(0, { 5,6,7,8,9 });
	NetMessage msgE(0, { 6,7,8,9,10 });
	bool Apressed = false;
	bool Bpressed = false;
	bool Cpressed = false;
	bool Dpressed = false;
	bool Epressed = false;
	while (true)
	{
		// hard coded get key state
		if (GetAsyncKeyState(0x41) < 0 && !Apressed && !Bpressed && !Cpressed && !Dpressed && !Epressed)
		{
			Apressed = true;
			std::cout << "'A' is pressed" << std::endl;
			client.m_messageOut.push_back(msgA);
			client.WriteMessage();
		}
		if (GetAsyncKeyState(0x41) == 0 && Apressed)
		{
			Apressed = false;
			std::cout << "'A' is released" << std::endl;
		}
		if (GetAsyncKeyState(0x42) < 0 && !Bpressed && !Apressed && !Cpressed && !Dpressed && !Epressed)
		{
			Bpressed = true;
			std::cout << "'B' is pressed" << std::endl;
			client.m_messageOut.push_back(msgB);
			client.WriteMessage();
		}
		if (GetAsyncKeyState(0x42) == 0 && Bpressed)
		{
			Bpressed = false;
			std::cout << "'B' is released" << std::endl;
		}
		if (GetAsyncKeyState(0x43) < 0 && !Cpressed && !Bpressed && !Apressed && !Dpressed && !Epressed)
		{
			Cpressed = true;
			std::cout << "'C' is pressed" << std::endl;
			client.m_messageOut.push_back(msgC);
			client.WriteMessage();
		}
		if (GetAsyncKeyState(0x43) == 0 && Cpressed)
		{
			Cpressed = false;
			std::cout << "'C' is released" << std::endl;
		}
		if (GetAsyncKeyState(0x44) < 0 && !Dpressed && !Bpressed && !Cpressed && !Apressed && !Epressed)
		{
			Dpressed = true;
			std::cout << "'D' is pressed" << std::endl;
			client.m_messageOut.push_back(msgD);
			client.WriteMessage();
		}
		if (GetAsyncKeyState(0x44) == 0 && Dpressed)
		{
			Dpressed = false;
			std::cout << "'D' is released" << std::endl;
		}
		if (GetAsyncKeyState(0x45) < 0 && !Epressed && !Bpressed && !Cpressed && !Dpressed && !Apressed)
		{
			Epressed = true;
			std::cout << "'E' is pressed" << std::endl;
			client.m_messageOut.push_back(msgE);
			client.WriteMessage();
		}
		if (GetAsyncKeyState(0x45) == 0 && Epressed)
		{
			Epressed = false;
			std::cout << "'E' is released" << std::endl;
		}

		client.Update();
	}
	return 0;
}