#pragma once

#include <config/HostConfig.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	class HostConfigParser : public AConfigParser
	{
	public:
		HostConfigParser(StreamData* streamData);

	protected:
		std::map<std::string, ICommand*> createKeywords();

	private:
		Webserver::HostConfig _data;
	};
}
