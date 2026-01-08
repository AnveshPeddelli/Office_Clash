#include <iostream>
#include <string>

#include "enet/enet.h"



class Client
{
public:
	Client();
	~Client();

public:
	void setHostAddressPort(const ENetAddress& address);
	bool checkConnectionStatus();
	bool startMessaging();

	
private:
	void setLocalClientIP();


private:
	char client_ip[32];

	ENetHost* client_m = nullptr;
	ENetPeer* peer_m = nullptr;
	ENetAddress serverAddress_m;
	int host_port_m;
	char server_ip[32];
};

