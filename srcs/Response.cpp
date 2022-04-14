#include <iostream>
#include <Response.hpp>


Response::Response() : _bodyStream(nullptr)
{
}

Response::Response(HttpStatusCode code) : _statusCode(code), _bodyStream(nullptr)
{
}

Response::Response(const Response &ref)
{
	*this =	ref;
}

Response::~Response()
{
	if (_bodyStream != nullptr)
		delete _bodyStream;
}

Response& Response::operator=(const Response &rhs)
{
	this->_headerFields = rhs._headerFields;
	this->_statusCode = rhs._statusCode;
	return (*this);
}

void Response::setBodyStream(std::ifstream* stream)
{
	_bodyStream = stream;
	stream->seekg(0, std::ios_base::end);
	addHeaderField("Content-Length", std::to_string(stream->tellg()));
}

void Response::setStatusCode(HttpStatusCode code)
{
	this->_statusCode = code;
}

void Response::addHeaderField(std::string key, std::string value)
{
	_headerFields.insert(std::pair<std::string, std::string>(key, value));
}

void Response::addHeaderFields()
{
	_headerFields.insert(std::pair<std::string, std::string>("Server", "Simply the best"));
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
	if (_bodyStream == nullptr || !_bodyStream->is_open())
		return nullptr;
	_bodyStream->seekg(0, std::ios_base::beg);
	return _bodyStream;
}
