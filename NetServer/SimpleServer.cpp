#include "NetServer.h"

enum CustomMessage
{
	PING_SERVER = 0,
	GET_ID,
	MESSAGE_ALL,
	DISCONNECT
};

int main()
{
	// construct a server obj
	NetServer server;
	// start the server
	server.Start();
	// start listening
	while (true)
	{
		server.Update();
	}
	return 0;
}