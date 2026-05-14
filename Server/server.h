#include <iostream>
#include "Export.h"

#if defined(OFFICECLASH_WITH_ENET)
#include "enet/enet.h"
#else
struct ENetHost;
#endif


class SERVER_API Server
{
public:
	Server();
	~Server();

	void run();

private:
	ENetHost* server_m = nullptr;


};

