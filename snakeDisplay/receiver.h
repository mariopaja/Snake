#ifndef RECEIVER
#define RECEIVER

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#pragma warning(disable:4996) 
#define BUFFER_LENGTH 512

#define PORT 8888


SOCKET receiver_socket;
sockaddr_in receiver_address;
int message_length;
int socket_length = sizeof(sockaddr_in);

void displaySetup()
{
	system("title Snake Display");

	//declaring the windows socket
	WSADATA wsaDATA;
	printf("Initialising Winsock...");

	//initiating windows socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0)
	{
		printf("Failed. Error Code: %d", WSAGetLastError());
		exit(0);
	}
	printf("Initialised.\n");

	//creating a UDP socket of the transmitter
	//when it is created it returns 0
	if ((receiver_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("Could not create socket: %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	receiver_address.sin_family = AF_INET;
	receiver_address.sin_addr.s_addr = INADDR_ANY;
	receiver_address.sin_port = htons(PORT);

	//connect to socket
	if (bind(receiver_socket, (sockaddr*)&receiver_address, sizeof(receiver_address)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code: %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done.");
}

//close socket
void clearDisplay()
{
	closesocket(receiver_socket);
	WSACleanup();
}

//this function receives the data and then in prints it
void receiveGameData()
{
	//declare a char array to hold the message
	char snakeCommand[BUFFER_LENGTH] = {};


	//check if message is received from that socket
	if (message_length = recvfrom(receiver_socket, snakeCommand, BUFFER_LENGTH, 0, (sockaddr*)&receiver_address, &socket_length) == SOCKET_ERROR)
	{
		printf("recvfrom() failed with error code: %d", WSAGetLastError());
		exit(0);
	}

	//if message is cls clean the console screen
	//else print the message(map)
	if (strcmp(snakeCommand, "cls") == 0)system("cls");
	else std::cout << snakeCommand << std::endl;
}

#endif
