#include "client.h"

Client::Client()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "Failed to Initialize ENet" << std::endl;
	}
	std::cout << "ENet Initialization Successfull" << std::endl;

	client_m = enet_host_create(NULL, 1, 1, 0, 0);
	if (!client_m)
	{
		std::cout << "Failed to Create ENet Client" << std::endl;
		return;
	}
	std::cout << "ENet Client Initialization Successfull" << std::endl;

}

Client::~Client()
{
}

void Client::setHostAddressPort(std::string& address, int port)
{
	enet_address_set_host(&client_address_m)
	
}


void Client::checkConnectionStatus()
{
	try
	{
		

	}
	catch (std::exception& ex)
	{
		std::string msg = "Exception Occured at Client : checkConnectionStatus -> ";
		std::cerr << msg + ex.what();
	}
}