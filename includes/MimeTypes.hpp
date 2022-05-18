#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

namespace Webserver
{
	class MimeTypes
	{
	public:
		MimeTypes();
		MimeTypes(const std::string& filePath);
		~MimeTypes();

		const std::string getMIMEType(const std::string& extension) const;

	private:
		void parse();

		std::map<const std::string, const std::string> _types;
		std::string _filePath;

	};
}
