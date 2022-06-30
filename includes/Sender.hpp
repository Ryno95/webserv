#pragma once

#include <responses/Response.hpp>

#include <iostream>

namespace Webserver
{
	/*
		Sender will send the currently assignd Response object.
		Before calling handle, first make sure that a Response object is set using hasResponse and setResponse.
	*/
	class Sender
	{
	public:
		Sender(int fd);
		Sender(Sender const& rhs);
		~Sender();
		Sender& operator=(Sender const& rhs);

		void handle();
		bool hasResponse() const;
		void setResponse(Response* response);

	private:
		enum state
		{
			START,
			SEND_HEADER,
			SEND_BODY,
			FINISHED
		};

		void setStream();
		void deleteResponse();

		int _fd;
		int _currentState;
		Response* _response;
		std::istream* _sendStream;
		char* _buffer;
	};
}
