#include <iostream>
#include <Response.hpp>


Response::Response() {}


Response::Response(HttpStatusCode code) : _statusCode(code)
{
}

Response::Response(const Response &ref)
{
	*this =	ref;
}

Response::~Response()
{
	if (_bodyStream.is_open())
		_bodyStream.close();
}

Response& Response::operator=(const Response &rhs)
{
	this->_headerFields = rhs._headerFields;
	this->_statusCode = rhs._statusCode;
	return (*this);
}

void Response::setStatusCode(HttpStatusCode code)
{
	this->_statusCode = code;
}

void Response::setBodyStream(std::string filePath)
{
	_bodyStream.open(filePath);
	if (_bodyStream.fail())
	{
		std::cout << "Failed to open the file" << std::endl;
		return;
	}

	_bodyStream.seekg(0, std::ios_base::end);
	_headerFields["Content-Length"] = std::to_string(_bodyStream.gcount());
}

void Response::addHeaderFields()
{
	_headerFields.insert(std::pair<std::string, std::string>("Server", "Simply the best"));
	_headerFields.insert(std::pair<std::string, std::string>("Content-Length", "0"));
	_headerFields.insert(std::pair<std::string, std::string>("Accept-Ranges", "bytes"));
}

std::stringstream	*Response::getHeaderStream()
{
	std::cout << "Header stream requested" << std::endl;
	std::map<std::string, std::string>::const_iterator cursor = _headerFields.begin();
	std::map<std::string, std::string>::const_iterator end = _headerFields.end();
	
	std::string	header = HTTPVERSION;
	header += " ";
	header += std::to_string(_statusCode.first);
	header += " ";
	header += _statusCode.second;
	while (cursor != end)
	{
		header += "\r\n";
		header += cursor->first;
		header += ": ";
		header += cursor->second;
		++cursor;
	}
	header += "\r\n\r\n";
	_headerStream << header;
	return (&_headerStream);
}


std::ifstream* Response::getBodyStream()
{
	std::cout << "Body stream requested" << std::endl;
	if (!_bodyStream.is_open())
		return nullptr;
	return (&_bodyStream);
}
