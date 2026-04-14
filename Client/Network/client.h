#include <iostream>
#include <string>
#include <cstdint>

#if defined(OFFICECLASH_WITH_ENET)
#include <enet/enet.h>
#else
struct ENetAddress
{
	std::uint32_t host = 0;
	std::uint16_t port = 0;
};

struct ENetHost;
struct ENetPeer;
#endif



class Client
{
public:
	Client();
	~Client();

public:
	void setHostAddressPort(const ENetAddress& address, std::string ip);
	bool checkConnectionStatus();
	bool startMessaging();

	
private:
	void setLocalClientIP();


private:
	char client_ip[32] = {};

	ENetHost* client_m = nullptr;
	ENetPeer* peer_m = nullptr;
	ENetAddress serverAddress_m{};
	int host_port_m = 0;
	char server_ip[32] = {};
};

