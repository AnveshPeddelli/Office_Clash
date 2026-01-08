#include <iostream>
#include "Export.h"

#include "enet/enet.h"


class SERVER_API Server
{
public:
	Server();
	~Server();

	void run();

private:
	ENetHost* server_m = nullptr;


};

