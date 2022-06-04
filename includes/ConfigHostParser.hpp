#pragma once
#include <AConfigParser.hpp>

namespace ConfigFileParser
{
	class ConfigHostParser : public AConfigParser
	{
	public:
		ConfigHostParser(StreamData streamData);
	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		std::vector<std::string> _names;
	};
}
