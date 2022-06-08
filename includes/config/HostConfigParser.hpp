#pragma once

#include <config/HostConfig.hpp>
#include <config/AConfigParser.hpp>
// #include <config/LocationConfigParser.hpp>

namespace Webserver
{
	class LocationConfigParser;
	class HostConfigParser : public AConfigParser, public IBranch<HostConfig, LocationConfigParser>
	{
	public:
		typedef HostConfig data_type;
		typedef LocationConfigParser child_type;

		HostConfigParser(StreamData* streamData);

	protected:
		virtual std::map<std::string, ICommand*> createKeywords();
		virtual void validate();

		void addProtectedKeywords(std::map<std::string, ICommand*>& keywords);

	private:
		data_type _data;
	};
}
