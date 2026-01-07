#include <iostream>
#include "Export.h"

#include "enet/enet.h"


class SERVER_API Server
{
public:
	Server();
	~Server();

	bool setHost();
	void checkConnectionStatus(ENetEvent _event);

private:
	ENetHost* server_m;
	ENetAddress address_m;

};

