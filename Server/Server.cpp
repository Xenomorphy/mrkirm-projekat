#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "Commands.h"
#include "Database.h"

int main(void)
{
	Database Db;
	Commands Ops;
	int CommandSize = 256;
	char Command[CommandSize];
	int ServerId, ClientId;
	int PortNum = 3333;
	struct sockaddr_in ServerAddr;
	socklen_t Size;

	// Initialise database.
	Db.InitDatabase();

	// Establish connection with client.
	ClientId = socket(AF_INET, SOCK_STREAM, 0);
	if(ClientId < 0)
	{
		std::cout << "Error creating the client socket." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Client socket created." << std::endl;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htons(INADDR_ANY);
	ServerAddr.sin_port = htons(PortNum);

	Size = sizeof(ServerAddr);
	if((bind(ClientId, (struct sockaddr*)&ServerAddr, Size)) < 0)
	{
		std::cout << "Error binding the client socket." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for client..." << std::endl;

	listen(ClientId, 1);
	ServerId = accept(ClientId, (struct sockaddr*)&ServerAddr, &Size);
	if(ServerId < 0)
	{
		std::cout << "Error accepting the client." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Client connected." << std::endl;

	Ops.CommandSize = CommandSize;
	Ops.ServerId = ServerId;

	// Main server loop.
	while(1)
	{
		// Get command from client.
		Size = recv(ServerId, Command, CommandSize, 0);
		if(Size == 0)
		{
			std::cout << "Error receiving message." << std::endl;
			exit(EXIT_FAILURE);
		}

		if(!strcmp(Command, "quit")) break;

		Ops.ParseCommand(Db, std::string(Command));
	}

	close(ServerId);
	close(ClientId);

	return 0;
}
