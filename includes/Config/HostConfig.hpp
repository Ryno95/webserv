#pragma once

#include <string>
#include <vector>

#include <Method.hpp>

struct HostConfig
{
	HostConfig& operator=(const HostConfig& ref)
	{
		(void)ref;
		return *this;
	}

	std::vector<std::string>	names;
	std::string					root;
	std::string					defaultIndex;
	std::string					defaultError;
	std::vector<Method::method>	acceptedMethods;
};
