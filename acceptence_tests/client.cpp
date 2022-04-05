#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <string>

void sendRequest(std::string request)
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
		
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
		return;

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		return;

	request += "\r\n";
	request += "host: testert\r\n\r\n";
	write(sock, request.c_str(), request.size());
	valread = read(sock, buffer, 1024);
}

int main(int argc, char** argv)
{
	while (argc > 1)
	{
		--argc;
		sendRequest(argv[argc]);
	}
}
