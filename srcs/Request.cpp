#include <Request.hpp>

#include <iostream>
#include <cctype>

Request::Request() {}

Request::Request(std::string query) : _query(query), _status(HttpStatusCodes::OK)
{
}

Request::~Request()
{
}

void Request::parseBody(size_t pos)
{
	std::cout << "PARSING BODY " << std::endl;
	std::cout << "BODY>>>\n" << &_query[pos] << "<<<\n";

}

bool Request::hasBodyField() const
{
	if (_method == POST)
	{
		std::cout << "POST METHOD\n";
		return true;
	}
	return false;
}

void Request::appendBody(const std::string &body)
{
	_body += body;
	std::cout << "BODY: " << _body << std::endl;
}

size_t Request::parseMethod()
{
	size_t pos;

	pos = _query.find(' ');
	if (pos == std::string::npos)
		throwError(HttpStatusCodes::BAD_REQUEST);
	std::string method = _query.substr(0, pos);
	if (method == "GET")
		_method = GET;
	else if (method == "POST")
		_method = POST;
	else if (method == "DELETE")
		_method = DELETE;
	else
		throwError(HttpStatusCodes::BAD_REQUEST);
	return pos;
}

size_t Request::parseTarget(size_t pos)
{
	size_t pos2;

	pos2 = _query.find(' ', pos);
	if (pos == std::string::npos || pos2 - pos == 0)
		throwError(HttpStatusCodes::BAD_REQUEST);
	_target = _query.substr(pos, (pos2 - pos));
	if (_target.size() > MAX_TARGET_LEN)
		throwError(HttpStatusCodes::URI_TOO_LONG);
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
		throwError(HttpStatusCodes::BAD_REQUEST);
	pos2 = pos + versionPrefixLen;

	pos = _query.find('.', pos2);
	if (pos == std::string::npos)
		throwError(HttpStatusCodes::BAD_REQUEST);

	tmp = _query.substr(pos2, pos - pos2);  // check if this section (major) ONLY contains digits
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!std::isdigit(tmp[i]))
			throwError(HttpStatusCodes::BAD_REQUEST);
	}
	major = std::atoi(tmp.c_str());
	if (major != HTTPVERSION_MAJOR)
		throwError(HttpStatusCodes::BAD_REQUEST);

	pos2 = _query.find('\r', 0);
	if (_query.find('\n', 0) - pos2 != 1) // there's no \n somewhere in the line, only at \r\n
		throwError(HttpStatusCodes::BAD_REQUEST);

	tmp = _query.substr(pos + 1, pos2 - pos - 1); // check if this section (minor) ONLY contains digits
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!std::isdigit(tmp[i]))
			throwError(HttpStatusCodes::BAD_REQUEST);
	}
	minor = std::atoi(tmp.c_str());
	if (minor != HTTPVERSION_MINOR)
		throwError(HttpStatusCodes::BAD_REQUEST);
	return pos2 + 2;
}

// request-line = method SP request-target SP HTTP-version CRLF
size_t Request::parseRequestLine()
{
	size_t pos;

	pos = parseMethod();
	pos = parseTarget(pos + 1);
	pos = parseVersion(pos + 1);
	
	return pos;
}

void	Request::addKeyValuePair(const std::string &src, size_t newLinePos)
{
	const size_t 	colonPos = src.find(COLON);
	int				i = 0;

	if (colonPos == std::string::npos || std::isspace(src[colonPos - 1]))
		throwError(HttpStatusCodes::BAD_REQUEST);
	
	std::string key = src.substr(i, colonPos);
	while(std::isspace(src[colonPos + CRLF_CHAR_COUNT + i]))
		++i;
	std::string value = src.substr(colonPos + CRLF_CHAR_COUNT + i, newLinePos);
	_headerFields.insert(std::pair<std::string, std::string>(key, value));
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

static bool	isTerminatorStr(const std::string str)
{
	const std::string	terminatorStr = "\r\n\r\n";

	return (str.compare(terminatorStr) == 0);
}

size_t Request::parseHeaderFields(size_t pos)
{
	size_t				next = 0, last = pos;
	std::string			trimmedLine;
	while ((next = _query.find(CRLF, last)) != std::string::npos)
	{
		trimmedLine = getTrimmedLine(_query.substr(last, next - last));
		addKeyValuePair(trimmedLine, next);
		if (isTerminatorStr(_query.substr(next, TERMINATOR_LEN)))
		{	
			// std::cout << "END REACHED" << std::endl;
			last = next + 4;
			break ;
		}
		last = next + CRLF_CHAR_COUNT;
	}
	// std::cout << "WHATS LEFT: " << _query.substr(last, _query.length()) << std::endl;
	return (last);
}

void Request::parse()
{
	// std::cout << "FULL REQUEST>>>" << _query << "<<<" << std::endl;
	size_t pos = parseRequestLine();
	pos = parseHeaderFields(pos);
	// parseBody(pos);
}

void Request::throwError(HttpStatusCode code)
{
	_status = code;
	throw std::runtime_error("Parse error");
}


HttpStatusCode Request::getStatus() const
{
	return _status;
}

std::string Request::getTarget() const
{
	return _target;
}

method Request::getMethod() const
{
	return _method;
}

size_t Request::getBodySize() const
{
	std::map<std::string, std::string>::const_iterator it = _headerFields.find("Content-Length");

	if (it != _headerFields.end())
		return std::atol(it->second.c_str());
	return -1;
}
