#pragma once
#include <AConfigParser.hpp>

namespace ConfigFileParser
{
	class ConfigLocationParser : public AConfigParser
	{
	public:
		ConfigLocationParser(StreamData streamData);
	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		std::string _route;
	};
}
