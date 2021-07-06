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

int main()
{
	// construct a server obj
	NetServer<Protocal> server;
	// start the server
	server.Start();
	// start listening
	while (true)
	{
		server.Update();
	}
	return 0;
}