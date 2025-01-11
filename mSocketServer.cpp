/*
                   CREATED BY CR33P3N
				        12.01.2025
		    SIMPLE MULTI CLIENT SOCKET SERVER
*/

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <vector>

//#pragma warning(disable: 4996)

std::vector<SOCKET> connections;  //vector for storing connections

void handleClient(SOCKET clientSocket) {
    char buffer[256]; 
    int bytesReceived; 

	//read data from client
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
		//echo data back to client
        send(clientSocket, buffer, bytesReceived, 0);
    }

    if (bytesReceived == 0) {
        std::cout << "Client disconnected.\n";
    }
    else if (bytesReceived == SOCKET_ERROR) {
        std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
    }
    closesocket(clientSocket);
}

int main(int argc, char* argv[]) {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1); // required version of the winsock library
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error initializing Winsock\n";
        exit(1);
    }

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP
    addr.sin_port = htons(1111);  //port
    addr.sin_family = AF_INET;    //ipv4

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //create socket
    if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout << "Binding failed with error: " << WSAGetLastError() << "\n";
        return 1;
    }

    if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listening failed with error: " << WSAGetLastError() << "\n";
        return 1;
    }
    std::cout << "Server here\n";
	//cycle for accepting clients
    while (true) {
		SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //wait for client
        if (newConnection == INVALID_SOCKET) {
            std::cout << "Failed to accept the client's connection.\n";
            continue;
        }

		//add new connection to vector
        connections.push_back(newConnection);
        std::cout << "Client connected! Starting new thread for client...\n";
        char msg[256] = "Welcome! You have connected to the server.\n"; //your text
        send(newConnection, msg, sizeof(msg), NULL);

		//create new thread for client
        std::thread clientThread(handleClient, newConnection);
		clientThread.detach(); //detach
    }
    closesocket(sListen);
    WSACleanup();

    return 0;
}
