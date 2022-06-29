#pragma once

#include <string>
#include <HeaderFields.hpp>

namespace Webserver
{
	class HeaderFieldParser
	{
	public:
		HeaderFieldParser();
		~HeaderFieldParser();

		HeaderFields parse(const std::string& headerFields);

	protected:
		HeaderFields _fields;

		void parseKeyValuePair(const std::string& line);

	};
}
