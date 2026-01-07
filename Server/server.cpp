#include "Server.h"

Server::Server()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "Failed to Initialize ENet" << std::endl;
	}
	std::cout << "ENet Initialization Successfull" << std::endl;
	

	server_m = enet_host_create(NULL, 10, 1, 0, 0);
	if (!server_m)
	{
		std::cout << "Failed to Create ENet Server" << std::endl;
		return;
	}
	std::cout << "ENet Server Initialization Successfull" << std::endl;

	address_m.host = ENET_HOST_ANY;
	address_m.port = 1234;
}

Server::~Server()
{

	enet_host_destroy(server_m);
	server_m = NULL;
	enet_deinitialize();
}

bool Server::setHost()
{
	server_m = enet_host_create(&address_m, 10, 1, 0, 0);
	
	if (!server_m)
	{
		std::cerr << "Failed to create ENet Server" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "********************* Server started... *********************" << std::endl;
	std::cout << "Server Address: "<< address_m.host << std::endl;
	std::cout << "Server Port: "<< address_m.port << std::endl;
	std::cout << "*************************************************************" << std::endl;

	return 0;
}


void Server::checkConnectionStatus(ENetEvent _event)
{
	try
	{
		while (true)
		{
			while (enet_host_service(server_m, &_event, 1000))
			{
				switch (_event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					std::cout << "Client Connected From " << _event.peer->address.host << ":" << _event.peer->address.port << std::endl;
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << _event.peer->address.host + " Disconnected" << std::endl;
					_event.peer->data = NULL;
					break;
				}
			}
		}

	}
	catch (std::exception& ex)
	{
		std::string msg = "Exception Occured at Server : checkConnectionStatus -> ";
		std::cerr << msg + ex.what();
	}
}