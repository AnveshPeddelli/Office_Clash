#include "client.h"

static bool enetInitialized = false;

Client::Client()
{
	if (!enetInitialized)
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "Failed to Initialize ENet" << std::endl;
		}
		
		enetInitialized = true;
		std::cout << "ENet initialized\n";
	}
}

Client::~Client()
{
	if (client_m)
		enet_host_destroy(client_m);

	client_m = nullptr;
	
	if (enetInitialized)
	{
		enet_deinitialize();
		enetInitialized = false;
		std::cout << "ENet De-Initialized\n";

	}
}


void Client::setHostAddressPort(const ENetAddress& address, std::string ip)
{
	serverAddress_m = address;
	std::cout << "serverPort: " << serverAddress_m.port << std::endl;
	//server_ip = ip.c_str();
	client_m = enet_host_create(NULL, 1, 2, 0, 0);
	auto stsa = enet_address_set_host(&serverAddress_m, ip.c_str());
	std::cout << "Stats: " << stsa << std::endl;

	if (!client_m)
	{
		std::cout << "Failed to Create ENet Client" << std::endl;
		return;
	}
	std::cout << "Connected to server IP: " << server_ip << " Port: " << address.port << std::endl;

	//Setting local ip address for display purposes
	setLocalClientIP();
}

bool Client::checkConnectionStatus()
{
	peer_m = enet_host_connect(client_m, &serverAddress_m, 1, 0);
	if (!peer_m)
	{
		std::cerr << client_ip << " Failed to connect to server!\n";
		return false;
	}

	auto stst = startMessaging();

	ENetEvent _event;
	if (enet_host_service(client_m, &_event, 5000) > 0 && _event.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cout << client_ip << " -> Connected to server [CS]\n";

		//Sending message to server
		const char* msg = "Hi Server!";
		ENetPacket* packet = enet_packet_create(msg, strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer_m, 0, packet);
		enet_host_flush(client_m);
		
		return true;
	}
	else
	{
		enet_peer_reset(peer_m);
		peer_m = nullptr;
		return false;
	}
}

void Client::setLocalClientIP()
{
	
	ENetAddress localAddress;
	enet_socket_get_address(client_m->socket, &localAddress);
	enet_address_get_host_ip(&localAddress, client_ip, sizeof(client_ip));
}

bool Client::startMessaging()
{
	std::string input;
	ENetEvent _event;

	while (true)
	{
		//continuous input check
		if (std::getline(std::cin, input))
		{
			if (input == "-1")
				break;

			ENetPacket* packet = enet_packet_create(input.c_str(), input.size() + 1, ENET_PACKET_FLAG_RELIABLE);
			
			enet_peer_send(peer_m, 0, packet);
			enet_host_flush(client_m);
		}

		//poll enet events
		while (enet_host_service(client_m, &_event, 10) > 0)
		{
			switch (_event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				std::cout << "Recieved: " << (char*)_event.packet->data << std::endl;
				enet_packet_destroy(_event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected from server\n";
				return false;
			}
		}
	}

	return true;

	//bool sig = true;
	//int i = 0;
	//ENetEvent _event;
	//
	//while (sig)
	//{
	//	std::cin >> input;
	//	if (input.size() == 1)
	//	{
	//		if (input[0] == '-1')
	//		{
	//			break;
	//		}
	//	}
	//	
	//	if (enet_host_service(client_m, &_event, 5000) > 0 && _event.type == ENET_EVENT_TYPE_CONNECT)
	//	{
	//		const char* msg = input.c_str();
	//		ENetPacket* packet = enet_packet_create(msg, strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE);
	//		enet_peer_send(peer_m, 0, packet);
	//		enet_host_flush(client_m);
	//	}
	//	//Listen for responses
	//	while (true)
	//	{
	//		while (enet_host_service(client_m, &_event, 1000) > 0)
	//		{
	//			if (_event.type == ENET_EVENT_TYPE_RECEIVE)
	//			{
	//				std::cout << "Recieved: " << (char*)_event.packet->data << std::endl;
	//				enet_packet_destroy(_event.packet);
	//			}
	//		}
	//	}
	//}
}





//****************************************** MAIN LOOP ******************************************//
//
//int main()
//{
//	Client _client;
//	ENetAddress address;
//	std::string ip;
//	int port;
//	std::cout << "Enter the Server IP Address: ";
//	std::getline(std::cin, ip);
//	std::cout << "Enter the Server Port: ";
//	std::cin >> port;
//
//
//	ENetHost cli;
//
//	address.port = port;
//	std::cout << "entered->ip: " << ip << std::endl;
//	std::cout << "entered->port: " << port << std::endl;
//
//	_client.setHostAddressPort(address, ip);
//	_client.checkConnectionStatus();
//	
//	//_client.startMessaging();
//	
//
//
//	std::cout << "EXITED CLIENT CODE...\n";
//
//	return 0;
//}