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
	_address.port = 8000;

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


//int main() {
//	bool rv = enet.Initialize();
//	CHECK(rv == true);
//
//	enet::ServerHost* server = enet.CreateServerHost(4242);
//	CHECK(server != NULL);
//
//	enet::Event* event = enet.CreateEvent();
//
//	printf("Server started.\n");
//
//	uint32_t timeout = 100;
//	std::vector<char> data;
//
//	bool is_running = true;
//	int counter = 0;
//
//	while (is_running) {
//		bool rv = server->Service(event, timeout);
//		CHECK(rv == true);
//
//		switch (event->GetType()) {
//		case enet::Event::TYPE_CONNECT: {
//			printf("Client %s:%u connected.\n",
//				event->GetPeer()->GetIp().c_str(),
//				event->GetPeer()->GetPort());
//
//			enet::Peer* peer = event->GetPeer();
//			char msg[] = "Ohaio!";
//			data.assign(msg, msg + sizeof(msg));
//			bool rv = peer->Send(&data[0], data.size());
//			CHECK(rv == true);
//
//			break;
//		}
//
//		case enet::Event::TYPE_RECEIVE: {
//			event->GetData(&data);
//			data.push_back(0);
//			printf("From %s:%u: %s\n",
//				event->GetPeer()->GetIp().c_str(),
//				event->GetPeer()->GetPort(), &data[0]);
//			break;
//		}
//
//		case enet::Event::TYPE_DISCONNECT: {
//			printf("Client %s:%u disconnected.\n",
//				event->GetPeer()->GetIp().c_str(),
//				event->GetPeer()->GetPort());
//
//			counter += 1;
//			if (counter == 3) {
//				is_running = false;
//			}
//
//			break;
//		}
//		}
//	}
//
//	printf("Server finished.\n");
//
//	delete event;
//
//	delete server;
//
//	return EXIT_SUCCESS;
//}
