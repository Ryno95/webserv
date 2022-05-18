#pragma once

#include <map>
#include <string>

namespace Webserver
{
	class HeaderFields
	{
	public:
		HeaderFields();
		~HeaderFields();

		void add();
		std::string operator[](const std::string& key);

	private:
		std::map<std::string, std::string> _map;
	};
}
