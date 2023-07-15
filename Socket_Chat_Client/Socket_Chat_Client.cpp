#include <iostream>
#include <WS2tcpip.h>
#include <winsock.h>

using namespace std;

int main(int argc, char* argv[])
{
	// Initialize WSA / load DLL

	SOCKET clientSocket;
	int port = 15000;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		cout << "WinSock dll not found\n";
		return 0;
	}
	else
	{
		cout << "WinSock dll found\n";
		cout << "Status : " << wsaData.szSystemStatus << endl;
	}

	// Set up client socket

	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else
		cout << "Socket is OK\n";

	//  Connect winth server

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, (PCSTR)("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		cout << "Connetion failed: " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else
		cout << "Server connect is OK\n";

	// Messeging to server and recivig confirmation

	bool exitTrigger = false;

	while (exitTrigger == false)
	{
		char buffer[300];
		cout << "Enter message : ";
		cin.getline(buffer, 300);
		if (strcmp(buffer, "quit")==0)
			exitTrigger = true;

		int byteCount = send(clientSocket, buffer, 300, 0);
		if (byteCount > 0)
			cout << "Send message : " << buffer << endl;
		else
		{
			cout << "Failed to send message : " << WSAGetLastError() << endl;
			WSACleanup();
		}

		byteCount = recv(clientSocket, buffer, 100, 0);
		if (byteCount > 0)
			cout << "Server : " << buffer << endl;
		else
		{
			cout << "Failed to recive message : " << WSAGetLastError() << endl;
			WSACleanup();
		}
	}

	// Close sockets
	system("pause");
	WSACleanup();
}

