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
		virtual std::map<std::string, ICommand*> createKeywords();
		void addProtectedKeywords(std::map<std::string, ICommand*>& keywords);

	private:
		Webserver::HostConfig _data;
	};
}
