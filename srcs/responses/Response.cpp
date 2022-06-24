#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <responses/Response.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <Webserv.hpp>

namespace Webserver
{
	Response::Response() :
		_statusCode(HttpStatusCodes::OK),
		_bodyStream(nullptr)
	{
	}

	Response::Response(HttpStatusCode code) :
		_statusCode(code),
		_bodyStream(nullptr)
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
	}

	Response& Response::operator=(const Response &rhs)
	{
		this->_statusCode = rhs._statusCode;
		return (*this);
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

		mimeType = Webserv::config().getMimeTypes().getMIMEType(fileExtensionWithoutDot);

		return mimeType;
	}

	std::istream *Response::getHeaderStream()
	{
		std::map<std::string, std::string>::const_iterator cursor = headersBegin();
		std::map<std::string, std::string>::const_iterator end = headersEnd();
		
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
		return &_headerStream;
	}

	void Response::addFile(const std::string& filePath)
	{
		std::ifstream* stream = new std::ifstream(filePath);
		setBodyStream(stream);

		if (!stream->is_open())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);

		createBodyHeaders(filePath); // probably not OK yet, because this is the full file path and not just the file name.
	}

	void Response::setBodyStream(std::istream* stream)
	{
		if (_bodyStream != nullptr)
			delete _bodyStream;

		_bodyStream = stream;
	}

	void Response::createBodyHeaders(const std::string &fileName)
	{
		getBodyStream()->seekg(0, std::ios_base::end);
		addHeader(Header::ContentLength, std::to_string(_bodyStream->tellg()));
		addHeader(Header::ContentType, getContentTypeHeader(fileName));
		getBodyStream()->seekg(0);
	}

	std::istream* Response::getBodyStream() const
	{
		return _bodyStream;
	}

	void Response::setStatusCode(HttpStatusCode code)
	{
		_statusCode = code;
	}
}
