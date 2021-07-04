#include "NetCommon/NetMessage.h"

struct Test
{
	char* a;
};

int main()
{
	NetMessage msg;
	uint32_t a, b, c;
	msg << 1 << 2;
	msg.Print();
	msg >> a >> b;
	msg.Print();
	std::cout << "a: " << a << " b: " << b << std::endl;
	return 0;
}