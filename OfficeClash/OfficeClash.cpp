#include <iostream>
#include "server.h"

int main()
{
	std::cout << "Hello world" << std::endl;
	Server host = Server();
	host.setHost();



	return 0;
}