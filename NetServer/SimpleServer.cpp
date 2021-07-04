#include "NetServer.h"

int main()
{
	// construct a server obj
	NetServer server;
	// start the server
	server.Start();
	// start listening
	while (true)
		server.Update();
	return 0;
}