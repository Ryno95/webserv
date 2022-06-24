#include <iostream>
#include <cctype>
#include <cstdlib>

#include <Request.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <Logger.hpp>

namespace Webserver
{
	Request::Request() {}

	Request::Request(std::string query) : _query(query), _status(HttpStatusCodes::OK)
	{
	}

	Request::~Request()
	{
	}

	void Request::appendBody(const std::string &body)
	{
		_body += body;
	}

	const std::string &Request::getBody() const
	{
		return (_body);
	}

	size_t Request::parseRequestMethod()
	{
		size_t pos;

		pos = _query.find(' ');
		if (pos == std::string::npos)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		_method = parseMethod(_query.substr(0, pos));
		if (_method == Method::INVALID)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		return pos;
	}

	size_t Request::parseUri(size_t pos)
	{
		size_t pos2;

		pos2 = _query.find(' ', pos);
		if (pos == std::string::npos || pos2 - pos == 0)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		_uri = Uri(_query.substr(pos, (pos2 - pos)));
		return pos2;
	}

	size_t Request::parseVersion(size_t pos)
	{
		const std::string	versionPrefix = "HTTP/";
		const int			versionPrefixLen = versionPrefix.size();
		size_t pos2;
		std::string tmp;
		int major, minor;


		if (_query.substr(pos, versionPrefixLen) != versionPrefix) // case insensitive?
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		pos2 = pos + versionPrefixLen;

		pos = _query.find('.', pos2);
		if (pos == std::string::npos)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		tmp = _query.substr(pos2, pos - pos2);  // check if this section (major) ONLY contains digits
		for (size_t i = 0; i < tmp.size(); i++)
		{
			if (!std::isdigit(tmp[i]))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		}
		major = std::atoi(tmp.c_str());
		if (major != HTTPVERSION_MAJOR)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		pos2 = _query.find('\r', 0);
		if (_query.find('\n', 0) - pos2 != 1) // there's no \n somewhere in the line, only at \r\n
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		tmp = _query.substr(pos + 1, pos2 - pos - 1); // check if this section (minor) ONLY contains digits
		for (size_t i = 0; i < tmp.size(); i++)
		{
			if (!std::isdigit(tmp[i]))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		}
		minor = std::atoi(tmp.c_str());
		if (minor != HTTPVERSION_MINOR)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		return pos2 + 2;
	}

	// request-line = method SP request-target SP HTTP-version CRLF
	size_t Request::parseRequestLine()
	{
		size_t pos;

		pos = parseRequestMethod();
		pos = parseUri(pos + 1);
		pos = parseVersion(pos + 1);
		
		return pos;
	}

	void	Request::parseKeyValuePair(const std::string &src, size_t newLinePos)
	{
		const size_t 	colonPos = src.find(COLON);
		int				i = 0;

		if (colonPos == std::string::npos || std::isspace(src[colonPos - 1]))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		
		std::string key = src.substr(i, colonPos);
		while(std::isspace(src[colonPos + CRLF_CHAR_COUNT + i]))
			++i;
		std::string value = src.substr(colonPos + CRLF_CHAR_COUNT + i, newLinePos);
		addHeader(key, value);
	}

	std::string	getTrimmedLine(std::string line)
	{
		std::string::iterator start = line.begin();
		std::string::iterator end = line.end();

		while (start != end && std::isspace(*start))
			++start;
		--end;
		while (std::distance(start, end) > 0 && std::isspace(*end))
			--end;
		return std::string(start, end + 1);
	}

	void Request::parseHeaderFields(size_t pos)
	{
		size_t				next = 0, last = pos;
		std::string			trimmedLine;
		while ((next = _query.find(CRLF, last)) != std::string::npos)
		{
			trimmedLine = getTrimmedLine(_query.substr(last, next - last));
			parseKeyValuePair(trimmedLine, next);
			if (isTerminatorStr(_query.substr(next, TERMINATOR_LEN)))
				break ;
			last = next + CRLF_CHAR_COUNT;
		}
	}

	void Request::parse()
	{
		size_t pos = parseRequestLine();
		std::cerr << this->getTarget() << std::endl;
		parseHeaderFields(pos);
		validate();
	}

	HttpStatusCode Request::getStatus() const
	{
		return _status;
	}

	/*
		The target of a uri is guarenteed to be at least "/"
	*/
	const std::string& Request::getTarget() const
	{
		return _uri.getResourcePath();
	}

	// RFC: 5.2 The Resource Identified by a Request
	std::string Request::getHost() const
	{
		if (_uri.isAbsolute())
			return _uri.getHost();

		std::string value;
		if (tryGetHeader(Header::Host, value))
			return value;
		WARN("No host could be determined, while this should've been caught during Request::validate()");
		throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
	}

	Method::method Request::getMethod() const
	{
		return _method;
	}

	size_t Request::getBodySize() const
	{
		std::string length;
		if (tryGetHeader(Header::ContentLength, length))
			return std::atol(length.c_str());
		return 0;
	}

	/*
		Set the status of this request, so it will not be executed and send the corresponding error to the client instead.
	*/
	void	Request::setStatus(HttpStatusCode status)
	{
		_status = status;
	}

	void Request::validate() const
	{
		if (_uri.getHost().size() == 0 && !containsHeader(Header::Host))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
	}
}
