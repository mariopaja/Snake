#ifndef TRANSMITTER
#define TRANSMITTER

#include <vector>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 


#define CONTROLLER_IP "127.0.0.1"
#define BUFFER_LENGTH 512
#define PORT 8888

SOCKET transmitter_socket;
sockaddr_in transmitter;
int message_length;
int socket_length = sizeof(sockaddr_in);

void controllerSetup()
{
	system("title Snake Controller");

	//declaring the windows socket
	WSADATA wsaDATA;
	printf("Initialising Winsock...");


	//initiating windows socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0)
	{
		printf("Failed. Error Code: %d", WSAGetLastError());
		//return 1;
	}
	printf("Initialised.\n");


	//creating a UDP socket of the transmitter
	//when it is created it returns 0
	if ((transmitter_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code: %d", WSAGetLastError());
		//return 2;
	}

	// setup address structure
	//make all bytes of transmitter 0
	memset((char*)&transmitter, 0, sizeof(transmitter));
	transmitter.sin_family = AF_INET; //sets the address family determines the format
	transmitter.sin_port = htons(PORT); // set the port
	transmitter.sin_addr.S_un.S_addr = inet_addr(CONTROLLER_IP); //set the IP of the socket
}

//transmit the game data  function
//it takes the map size and the map as a parameter
void sendGameData(int mapSize, std::vector<std::vector<int>> vec)
{
	//send clear system
	//receiver will use this information to clean the display
	if (sendto(transmitter_socket, "cls", strlen("cls"), 0, (sockaddr*)&transmitter, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code: %d", WSAGetLastError());
	}

	//Loop used to go through map lines and transmit the data to the snake display
	for (int i = 0; i < mapSize; i++)
	{
		char Map[BUFFER_LENGTH] = { "" };// create a temporary char array
		for (int j = 0; j < mapSize; j++)
		{
			//check map element
			//add map element to the end of the char array
			if (vec[i][j] == 1)strcat(Map, "#");
			else if (vec[i][j] == 3)strcat(Map, "*");
			else if (vec[i][j] == 2)strcat(Map, "s");
			else if (vec[i][j] == 6)strcat(Map, "h");
			else if (vec[i][j] == 0)strcat(Map, " ");
			else strcat(Map, "X");

		}

		//transmit data 
		if (sendto(transmitter_socket, Map, strlen(Map), 0, (sockaddr*)&transmitter, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code: %d", WSAGetLastError());
			//return 3;
		}

	}

}
void closeGame()
{
	closesocket(transmitter_socket);
	WSACleanup();
}

#endif