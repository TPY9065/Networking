#include "NetCommon/NetMessage.h"

enum Test
{
	A = 0,
	B
};

int main()
{
	std::cout << Test::A << std::endl;
	std::cout << (Test)-1 << std::endl;
	return 0;
}