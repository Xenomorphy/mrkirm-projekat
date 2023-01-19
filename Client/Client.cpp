#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

int main(void)
{
	int ReturnCheck;
	int ClientId;
	int PortNum = 3333;
	int MessageSize = 256;
	char Message[MessageSize];
	std::string IP = "127.0.0.1";
	struct sockaddr_in ServerAddr;

	// Establish connection with server.
	ClientId = socket(AF_INET, SOCK_STREAM, 0);
	if(ClientId < 0)
	{
		std::cout << "Error creating the client socket." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Client socket created." << std::endl;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PortNum);
	ServerAddr.sin_addr.s_addr = inet_addr(IP.c_str());

	ReturnCheck = connect(ClientId, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if(ReturnCheck < 0)
	{
		std::cout << "Error connectin to the server." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Connected to server." << std::endl;

	// Main loop.
	while(1)
	{
		std::string Temp;
		getline(std::cin, Temp);
		strcpy(Message, Temp.c_str());

		send(ClientId, Message, MessageSize, 0);
		if(!strcmp(Message , "quit")) break;
		strcpy(Message, "");

		recv(ClientId, Message, MessageSize, 0);
		std::cout << Message << std::endl;
		strcpy(Message, "");
	}

	close(ClientId);
}
