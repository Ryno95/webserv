#pragma once
#include <AConfigParser.hpp>

namespace ConfigFileParser
{
	class ConfigServerParser : public AConfigParser
	{
	public:
		ConfigServerParser(StreamData streamData);
	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		uint _port;
	};
}
