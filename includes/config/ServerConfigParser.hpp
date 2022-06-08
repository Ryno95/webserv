#pragma once

#include <config/ServerConfig.hpp>
#include <config/AConfigParser.hpp>
#include <config/HostConfigParser.hpp>

namespace Webserver
{
	class ServerConfigParser : public AConfigParser, public IBranch<ServerConfig, HostConfigParser>
	{
	public:
		typedef ServerConfig data_type;
		typedef HostConfigParser child_type;

		ServerConfigParser(StreamData* streamData);
		data_type* getDataStruct() const;

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();

		data_type _data;
	};
}
