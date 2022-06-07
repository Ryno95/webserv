#pragma once

#include <config/HostConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class HostConfigParser : public AConfigParser
	{
	public:
		HostConfigParser(StreamData streamData);

	protected:
		std::map<std::string, ICommand*> initKeywords();

	private:
		Webserver::HostConfig _data;
	};
}
