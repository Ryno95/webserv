#pragma once

#include <string>
#include <vector>

#include <Method.hpp>
#include <config/AConfig.hpp>

struct HostConfig : public AConfig
{
	HostConfig();
	HostConfig(const HostConfig& ref);
	~HostConfig();
	HostConfig& operator=(const HostConfig& ref);

	std::vector<std::string>	names;
	std::vector<std::string>	locations;
	bool						autoIndexEnabled;
	std::string					root;
	std::string					defaultIndex;
	std::string					defaultError;
	std::vector<Method::method>	acceptedMethods;

private:
	map_type fillVariablesMap();

};
