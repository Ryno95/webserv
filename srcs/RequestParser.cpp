#include <RequestParser.hpp>
#include <HeaderFieldParser.hpp>
#include <Exception.hpp>

namespace Webserver
{
	RequestParser::RequestParser()
	{
	}

	RequestParser::~RequestParser()
	{
	}

	Request RequestParser::parse(const std::string& query)
	{
		_rawQuery = query;
		size_t pos = parseRequestLine();
		setHeaderFields(HeaderFieldParser().parse(_rawQuery.substr(pos)));
		validate();
		return static_cast<Request>(*this);
	}

	/*
		request-line = method SP request-target SP HTTP-version CRLF
	*/
	size_t RequestParser::parseRequestLine()
	{
		size_t pos;

		pos = parseRequestMethod();
		pos = parseUri(pos + 1);
		pos = parseVersion(pos + 1);
		
		return pos;
	}

	size_t RequestParser::parseVersion(size_t pos)
	{
		const std::string	versionPrefix = "HTTP/";
		const int			versionPrefixLen = versionPrefix.size();
		size_t pos2;
		std::string tmp;
		int major, minor;


		if (_rawQuery.substr(pos, versionPrefixLen) != versionPrefix) // case insensitive?
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		pos2 = pos + versionPrefixLen;

		pos = _rawQuery.find('.', pos2);
		if (pos == std::string::npos)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		tmp = _rawQuery.substr(pos2, pos - pos2);  // check if this section (major) ONLY contains digits
		for (size_t i = 0; i < tmp.size(); i++)
		{
			if (!std::isdigit(tmp[i]))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		}
		major = std::atoi(tmp.c_str());
		if (major != HTTPVERSION_MAJOR)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		pos2 = _rawQuery.find('\r', 0);
		if (_rawQuery.find('\n', 0) - pos2 != 1) // there's no \n somewhere in the line, only at \r\n
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		tmp = _rawQuery.substr(pos + 1, pos2 - pos - 1); // check if this section (minor) ONLY contains digits
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

	size_t RequestParser::parseRequestMethod()
	{
		size_t pos;

		pos = _rawQuery.find(' ');
		if (pos == std::string::npos)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		_method = parseMethod(_rawQuery.substr(0, pos));
		if (_method == Method::INVALID)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		return pos;
	}

	size_t RequestParser::parseUri(size_t pos)
	{
		size_t pos2;

		pos2 = _rawQuery.find(' ', pos);
		if (pos == std::string::npos || pos2 - pos == 0)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
		_uri = Uri(_rawQuery.substr(pos, (pos2 - pos)));
		return pos2;
	}

	void RequestParser::validate() const
	{
		if (_uri.getHost().size() == 0 && !containsHeader(Header::Host))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);
	}
}
