#include <HeaderFieldParser.hpp>
#include <Utility.hpp>
#include <HttpStatusCode.hpp>
#include <Exception.hpp>
#include <Logger.hpp>

#include <string>

#define COLON 			":"

namespace Webserver
{
	HeaderFieldParser::HeaderFieldParser() : _endl("\r\n")
	{
	}

	HeaderFieldParser::~HeaderFieldParser()
	{
	}

	void	HeaderFieldParser::parseKeyValuePair(const std::string &line)
	{
		const size_t 	colonPos = line.find(COLON);

		if (colonPos == std::string::npos || colonPos == 0 || colonPos == line.size() - 1 || std::isspace(line[colonPos - 1]))
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		std::string value = removeLeadingWhitespace(line.substr(colonPos + 1));
		_fields.addHeader(line.substr(0, colonPos), value);
	}

	HeaderFields HeaderFieldParser::parse(const std::string& headerFields)
	{
		size_t next, prev = 0;
		std::string line;

		while ((next = headerFields.find(_endl, prev)) != std::string::npos)
		{
			line = trimString(headerFields.substr(prev, next - prev));
			if (line.size() == 0)
				break;
			parseKeyValuePair(line);
			prev = next;
		}
		return _fields;
	}

	HeaderFieldParser& HeaderFieldParser::setEndl(const std::string& endl)
	{
		_endl = endl;
		return *this;
	}
}
