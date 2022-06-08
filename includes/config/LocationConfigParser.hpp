#pragma once

#include <config/HostConfigParser.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public HostConfigParser
	{
	public:
		typedef LocationConfig data_type;

		LocationConfigParser(StreamData* streamData, const std::string& pattern);

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();

		data_type _data;
	};
}
