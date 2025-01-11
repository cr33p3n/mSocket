//#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

//#pragma warning(disable: 4996)

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); //required version of the winsock library
	if (WSAStartup(DLLVersion, &wsaData) != 0) {     //1 required version of the winsock library 2 is link of structure of wsaData
		std::cout << "Error\n";
		exit(1);
	}
	//information about the socket
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip address
	addr.sin_port = htons(1111); //port
	addr.sin_family = AF_INET; //ipv4 socket

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL); //create socket
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Failed to connect to the server.\n";
	}
	else {
		std::cout << "Connected to the server!\n";
	}
	char msg[256];
	recv(connection, msg, sizeof(msg), NULL);
	std::cout << "Server: " << msg << std::endl;

	system("pause");
}
