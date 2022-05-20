#pragma once

#include <string>
#include <vector>

#include <Method.hpp>
#include <config/AConfig.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	struct HostConfig : public AConfig
	{
		HostConfig();
		HostConfig(const HostConfig& ref);
		~HostConfig();
		HostConfig& operator=(const HostConfig& ref);

		void	validate() const;

		std::vector<std::string>	names;
		std::vector<LocationConfig>	locations;
		bool						autoIndexEnabled;
		std::string					root;
		std::string					defaultIndex;
		std::string					defaultError;
		std::vector<Method::method>	acceptedMethods;

	private:
		map_type fillVariablesMap();

	};
}
