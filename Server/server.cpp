#include "Server.h"

Server::Server()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "Failed to Initialize ENet" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	if(server_m)
		enet_host_destroy(server_m);
	
	server_m = NULL;
	enet_deinitialize();
	std::cout << "SERVER DESTROYED" << std::endl;
}

void Server::run()
{
	ENetAddress _address;
	_address.host = ENET_HOST_ANY;
	_address.port = 1234;

	server_m = enet_host_create(&_address, 20, 2, 0, 0);
	if (!server_m)
	{
		std::cout << "Failed to start ENet Server" << std::endl;
		return;
	}
	std::cout << "********************* Server started... *********************" << std::endl;
	std::cout << "Server Address: "<< _address.host << std::endl;
	std::cout << "Server Port: "<< _address.port << std::endl;
	std::cout << "*************************************************************" << std::endl;
	std::cout << std::endl;
	std::cout << "[SERVER] Listening on port " << _address.port << std::endl;

	ENetEvent _event;
	while (true)
	{
		while (enet_host_service(server_m, &_event, 1000) > 0)  //if(==0)->timeout | if(>0)->evnet_recieved | if(<0)->error   <-- remember this
		{
			switch (_event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Client Connected From " << _event.peer->address.host << ":" << _event.peer->address.port << std::endl;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				std::cout << "Message from Clinet: " << (char*)_event.packet->data << std::endl;
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Client Disconnected" << std::endl;
				break;
			}
		}

	}
	std::cout << "waiting finished..." << std::endl;
	
	
	return;
}



//****************************************** MAIN LOOP ******************************************//

int main()
{
	Server _server;
	_server.run();

	return 0;
}