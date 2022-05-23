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

	Response::Response(HttpStatusCode code) : _statusCode(code), _bodyStream(nullptr)
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
		// this->_headerFields = rhs._headerFields;
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

	// void Response::addHeaderField(const std::string& key, const std::string& value)
	// {
	// 	_headerFields.insert(std::pair<std::string, std::string>(key, value));
	// }

	void Response::addConstantHeaderFields()
	{
		addHeader("Server", SERVER_NAME);
		addHeader("Accept-Ranges", "bytes");
		addHeader("Date", getTimeStamp());
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
		if (!_bodyStream->is_open()) // throw execption
			throw SystemCallFailedException("Response file not opened");
		getBodyStream()->seekg(0, std::ios_base::end);
		addHeader("Content-Length", std::to_string(_bodyStream->tellg()));
		addHeader("Content-Type", getContentTypeHeader(fileName));
	}

	std::stringstream	*Response::getHeaderStream()
	{
		std::map<std::string, std::string>::const_iterator cursor = begin();
		std::map<std::string, std::string>::const_iterator end = end();
		
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
		DEBUG("\n" + header);
		header += "\r\n\r\n";
		_headerStream << header;
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
