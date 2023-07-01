#include <iostream>
#include <string>
#include <conio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

SOCKET connection;
bool gl_isEndLoop = true;

void clientHandler() {
	// receive message clients from server
	// speaking clients
	char buf[256];
	while(true) {
		// pass name and output
		recv(connection, buf, sizeof(buf), 0);
		std::cout << "[Client][1]" << buf << std::endl;
	}
}

int main(int argc, char* argv[]) {
	setlocale(0, "");
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(50000);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	
	// First string, send to server
	std::cout << "Connected!\n";

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)clientHandler, 0, 0, 0);

	char data[256];

	while (true) {
		getch();
		std::cout << "Input you data: ";
		std::cin >> data;
		send(connection, data, sizeof(data), 0);
		Sleep(10);
	} 

	system("pause");
	return 0;
}
