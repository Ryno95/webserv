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
	buffer += std::string(target);
	buffer += " HTTP/1.1\r\n";
	buffer += "host: pipeline client\r\n";
	buffer += "\r\n";
	return buffer;
}

void sendRequest(std::string request)
{
	send(sockFd, request.c_str(), request.size(), 0);
	std::cout << "Sent: " << request << std::endl;
}

void readRequests()
{
	// bytesRead = read( sockFd , buffer, 1024);
	// printf("%s\n",buffer );
}

int main(int argc, char const *argv[])
{
	int valread;
	struct sockaddr_in serv_addr;

	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
		
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	sendRequest(makeGetRequest("index"));
	sendRequest(makeGetRequest("hallo"));
	// sendRequest(makeGetRequest("index") + makeGetRequest("hallo"));
	usleep(1000000);
}
