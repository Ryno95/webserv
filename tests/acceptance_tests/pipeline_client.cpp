#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

#define PORT 8080

int sockFd;

std::string makeGetRequest(char const* target)
{
	std::string buffer;

	buffer += "GET ";
	buffer += target;
	buffer += " HTTP/1.1\r\n";
	buffer += "host: pipeline client\r\n";
	buffer += "\r\n";
	return buffer;
}

void sendRequest(std::string request)
{
	send(sockFd, request.c_str(), request.size(), 0);
}

void readRequests()
{
	char buffer[1024];
	while (1)
	{
		int bytes = read(sockFd , buffer, 1023);
		if (bytes > 0)
		{
			buffer[bytes] = '\0';
			std::cout << buffer << std::endl;
		}
		else
			break;
	}
	std::cout << "Done receiving responses." << std::endl;
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		std::cout << "Supply one int as argument: the amount of connections to make in the pipeline." << std::endl;
		return 1;
	}

	int CONNECTION_COUNT = atoi(argv[1]);

	int valread;
	struct sockaddr_in serv_addr;


	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket error");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
		
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{
		perror("Invalid address");
		return -1;
	}

	if (connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Connection failed");
		return -1;
	}

	for (size_t i = 0; i < CONNECTION_COUNT; i++)
	{
		sendRequest(makeGetRequest("index.html"));
	}
	std::cout << "Done sending " << CONNECTION_COUNT << " requests, waiting for responses..." << std::endl << std::endl;
	readRequests();
}
