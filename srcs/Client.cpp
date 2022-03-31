#include <Client.hpp>
#include <defines.hpp>

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#define BUFFER_SIZE 9

Client::Client(int fd) : _fd(fd)
{
}

Client::~Client()
{
}

Client::state Client::recvRequest()
{
	int receivedBytes, prevSize;

	prevSize = _buffer.size();
	_buffer.resize(_buffer.size() + BUFFER_SIZE);
	receivedBytes = recv(_fd, &_buffer.front() + prevSize, BUFFER_SIZE, 0);

	if (receivedBytes == 0)
		return DISCONNECTED;
	if (receivedBytes != SYSTEM_ERR)
	{
		/*
			If we find \r\n\r\n, that means end of HEADER. We now need to determine if we have to read a body.
			RFC section 4.4 states:
				2.	If a Transfer-Encoding header field (section 14.41) is present and
					has any value other than "identity", then the transfer-length is
					defined by use of the "chunked" transfer-coding (section 3.6),
					unless the message is terminated by closing the connection.

				3.	If a Content-Length header field (section 14.13) is present, its
					decimal value in OCTETs represents both the entity-length and the
					transfer-length. The Content-Length header field MUST NOT be sent
					if these two lengths are different (i.e., if a Transfer-Encoding
					header field is present). If a message is received with both a
					Transfer-Encoding header field and a Content-Length header field,
					the latter MUST be ignored.

					For compatibility with HTTP/1.0 applications, HTTP/1.1 requests
					containing a message-body MUST include a valid Content-Length header
					field unless the server is known to be HTTP/1.1 compliant. If a
					request contains a message-body and a Content-Length is not given,
					the server SHOULD respond with 400 (bad request) if it cannot
					determine the length of the message, or with 411 (length required) if
					it wishes to insist on receiving a valid Content-Length.

				In HTTP, the only unsafe characteristic of message-bodies is the difficulty in
				determining the exact body length (section 7.2.2), or the desire to
				encrypt data over a shared transport.

				The Internet Assigned Numbers Authority (IANA) acts as a registry for
				transfer-coding value tokens. Initially, the registry contains the
				following tokens: "chunked" (section 3.6.1), "identity" (section
				3.6.2), "gzip" (section 3.5), "compress" (section 3.5), and "deflate"
				(section 3.5).

				A server which receives an entity-body with a transfer-coding it does
				not understand SHOULD return 501 (Unimplemented), and close the
				connection. A server MUST NOT send transfer-codings to an HTTP/1.0
				client.

				All HTTP/1.1 applications MUST be able to receive and decode the
				"chunked" transfer-coding, and MUST ignore chunk-extension extensions
				they do not understand.


			Summary:
			[Transfer-Encoding] is used to describe how we can determine the end of the body.
			Valid encodings are:
				- chunked		https://datatracker.ietf.org/doc/html/rfc2616#section-3.6.1
				- identity		https://datatracker.ietf.org/doc/html/rfc2616#section-3.5
				- gzip			^
				- compress		^
				- deflate		^
			Other encoding received? respond 501 (Unimplemented) and disconnect


			[Content-Length] is a different way of describing the length of the body.
			It's a number, describing how many bytes to read.

				1. Read until the blank line, so the whole header is received.
				2. In the header fields, search for [Transfer-Encoding] or [Content-Length]
					! Only if header field is present
					3. Keep on reading with either protocol [Transfer-Encoding], or read the amount of bytes [Content-Length]
				4. Process request
		*/
		if (_buffer.size() >= 4 && _buffer.find("\r\n\r\n") != std::string::npos)
			return RECV_DONE;
	}
	else
		perror("Read error");
	return CONTINUE;
}

bool Client::handleRequest()
{
	state st = recvRequest();

	if (st == CONTINUE)
		return true;
	else if (st == DISCONNECTED)
		return false;
	Request request(_buffer);
	try
	{
		request.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return true;
}

bool Client::sendResponse()
{
	return true;
}
