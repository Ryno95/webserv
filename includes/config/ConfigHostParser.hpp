#pragma once

#include <config/HostConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class ConfigHostParser : public AConfigParser
	{
	public:
		ConfigHostParser(StreamData streamData);

	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		Webserver::HostConfig _data;
	};
}