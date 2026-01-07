#include <iostream>

#include "enet/enet.h"



class Client
{
public:
	Client();
	~Client();

	void setHostAddressPort(std::string& address, int port);
	void checkConnectionStatus();

private:
	ENetHost* client_m;
	ENetPeer* peer_m;
	ENetAddress address_m;
	int host_port_m;
};

