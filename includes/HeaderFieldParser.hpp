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
		HeaderFieldParser& setEndl(const std::string& endl);

	protected:
		HeaderFields _fields;

		void parseKeyValuePair(const std::string& line);

	private:
		std::string _endl;
	};
}
