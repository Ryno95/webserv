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
		virtual void validate();

		void addProtectedKeywords(std::map<std::string, ICommand*>& keywords);

	private:
		HostConfig _data;
	};
}
