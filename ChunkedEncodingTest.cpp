#include <string>
#include <sstream>
#include <iostream>

std::string processBodyChunked(std::string& _buffer)
{
	int _bodySize = 0, _bodyBytesReceived = 0;
	// _bodyBytesReceived	= amount of bytes received in this chunk
	// _bodySize			= amount to receive in this chunk

	size_t pos = 0, prev = 0;
	std::string line;
	std::string body;

	while (true)
	{
		if (_bodySize == 0) // get next chunk size
		{
			if ((pos = _buffer.find("\r\n")) == std::string::npos)
			{
				std::cout << "receive more bytes first" << std::endl;
				break;
			}


			std::stringstream stream;
			stream << std::hex << _buffer.substr(0, pos);
			stream >> _bodySize;

			_buffer.erase(0, pos + 2);

			std::cout << "Bodysize: " << _bodySize << std::endl;

			if (_bodySize == 0)
			{
				std::cout << "END" << std::endl;
				break;
			}
		}
		else if (_buffer.size() >= _bodySize + 2)
		{
			if (_buffer.substr(_bodySize, 2) != "\r\n")
			{
				std::cout << "ERROR" << std::endl;
				break;
			}

			body += _buffer.substr(0, _bodySize);
			_buffer.erase(0, _bodySize + 2);

			_bodySize = 0;
		}
		else
		{
			std::cout << "receive more bytes first" << std::endl;
			break;
		}
	}
	return body;
}

int main()
{
	std::string inputStream, body;

	inputStream = "4\r\nWiki\r\n6\r\npedia \r\nE\r\nin \r\n\r\nchunks.\r\n0\r\n\r\n";
	body = processBodyChunked(inputStream);
	std::cout << "Body: " << body << std::endl;
	std::cout << "left of stream: " << inputStream << std::endl;

	inputStream = "7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n\r\n";
	body = processBodyChunked(inputStream);
	std::cout << "Body: " << body << std::endl;
	std::cout << "left of stream: " << inputStream << std::endl;
}
