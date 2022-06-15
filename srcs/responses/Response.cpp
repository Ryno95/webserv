#include <iostream>
#include <Logger.hpp>
#include <responses/Response.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <config/GlobalConfig.hpp>

namespace Webserver
{
	Response::Response() : _bodyStream(nullptr)
	{
	}

	Response::Response(HttpStatusCode code) : _cgiStream(nullptr), _statusCode(code), _bodyStream(nullptr)
	{
		addConstantHeaderFields();
	}

	Response::Response(const Response &ref)
	{
		*this =	ref;
	}

	Response::~Response()
	{
		if (_bodyStream != nullptr)
			delete _bodyStream;
		if (_cgiStream != nullptr)
			delete _cgiStream;
	}

	Response& Response::operator=(const Response &rhs)
	{
		this->_statusCode = rhs._statusCode;
		return (*this);
	}

	void Response::setBodyStream(std::ifstream* stream)
	{
		_bodyStream = stream;
	}

	void Response::setStatusCode(HttpStatusCode code)
	{
		this->_statusCode = code;
	}

	void Response::addConstantHeaderFields()
	{
		addHeader(Header::Server, SERVER_NAME);
		addHeader(Header::AcceptRanges, "bytes");
		addHeader(Header::Date, getTimeStamp());
	}

	static std::string getContentTypeHeader(const std::string &fileName)
	{
		const size_t	extensionIndex = fileName.find_last_of(".");
		std::string		fileExtensionWithoutDot;
		std::string 	mimeType;

		if (extensionIndex != std::string::npos)
			fileExtensionWithoutDot = fileName.substr(extensionIndex + 1, std::string::npos);

		mimeType = GlobalConfig::get().mimeTypes.getMIMEType(fileExtensionWithoutDot);

		return mimeType;
	}

	void Response::createContentHeaders(const std::string &fileName)
	{
		if (!_bodyStream->is_open()) // throw exception
			throw SystemCallFailedException("Response file not opened");
		getBodyStream()->seekg(0, std::ios_base::end);
		addHeader(Header::ContentLength, std::to_string(_bodyStream->tellg()));
		addHeader(Header::ContentType, getContentTypeHeader(fileName));
	}

	std::stringstream	*Response::getHeaderStream()
	{
		std::map<std::string, std::string>::const_iterator cursor = headersBegin();
		std::map<std::string, std::string>::const_iterator end = headersEnd();
		
		_headerStream << HTTPVERSION;
		_headerStream << " ";
		_headerStream << std::to_string(_statusCode.first);
		_headerStream << " ";
		_headerStream << _statusCode.second;
		while (cursor != end)
		{
			_headerStream << "\r\n";
			_headerStream << cursor->first;
			_headerStream << ": ";
			_headerStream << cursor->second;
			++cursor;
		}
		_headerStream << "\r\n\r\n";
		// __headerStream << header;
		return (&_headerStream);
	}

	std::ifstream* Response::getBodyStream()
	{
		if (_bodyStream == nullptr || !_bodyStream->is_open())
			return nullptr;
		_bodyStream->seekg(0, std::ios_base::beg);
		return _bodyStream;
	}
}
