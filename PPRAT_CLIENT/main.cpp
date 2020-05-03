#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <ctime>
#include "clientInfo.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Client {
	time_t StartupDate;
	string ClientName;
	string ClientUsername;
	string OSInfo;
	string UniqueID;
};



void main()
{
	time_t now = time(0);


	Client cl;
	cl.ClientName = GetPCName();
	cl.OSInfo = GetWindowsVersionString();
	cl.ClientUsername = GetPCUsername();
	cl.StartupDate = now;
	cl.UniqueID = "689480d9ac1011e9b3eb806e6f6e6963";


	cout << cl.ClientName << " " << cl.ClientUsername << " " << cl.OSInfo << " " << cl.StartupDate << " " << cl.UniqueID <<  endl;

	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;

	while (true)
	{
		//// Prompt the user for some text
		//cout << "> ";
		//getline(cin, userInput);

		//if (userInput.size() > 0)		// Make sure the user has typed in something
		//{
		//	// Send the text
		//	int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		//	if (sendResult != SOCKET_ERROR)
		//	{
				// Wait for response

				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
					cout << sizeof(cl) << endl;
					int sendResult = send(sock, reinterpret_cast<char*>(&cl), sizeof(Client) + 1, 0);
					if (sendResult != SOCKET_ERROR)
					{
						cerr << "ERROR WHILE SENDING DATA!" << endl;
					}
				}
	/*		}
		}*/

	}

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}