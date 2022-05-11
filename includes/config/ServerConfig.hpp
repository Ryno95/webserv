#pragma once

#include <vector>

#include <config/HostConfig.hpp>
#include <config/AConfig.hpp>

namespace Webserver
{
	typedef unsigned int uint;

	struct ServerConfig : public AConfig
	{
		ServerConfig();
		ServerConfig(const ServerConfig& ref);
		~ServerConfig();
		ServerConfig& operator=(const ServerConfig& ref);

		uint					port;
		std::vector<HostConfig>	hosts;

		void		validate() const;

	private:
		map_type 	fillVariablesMap();

		static const int	_maxPortNumber = 65535;
	};
}
