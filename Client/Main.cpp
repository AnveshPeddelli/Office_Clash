#include "Graphics/graphics.h"

//int main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	Graphics graphics;


	if (!graphics.initWindow(hInstance, nCmdShow))
		return -1;
	if (!graphics.initDirectX())
		return -1;

	while (graphics.isRunning())
	{
		graphics.renderFrame();
	}

	graphics.shutdown();
	return 0;
}




//****************************************** MAIN LOOP ******************************************//

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