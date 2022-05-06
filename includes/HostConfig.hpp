#pragma once

#include <string>
#include <vector>

#include <Method.hpp>

struct HostConfig
{
	HostConfig() {}

	HostConfig(const HostConfig& rhs)
	{
		*this = rhs;
	}

	HostConfig& operator=(const HostConfig& ref)
	{
		names = ref.names;
		root = ref.root;
		defaultIndex = ref.defaultIndex;
		defaultError = ref.defaultError;
		acceptedMethods = ref.acceptedMethods;
		return *this;
	}

	std::vector<std::string>	names;
	std::string					root;
	std::string					defaultIndex;
	std::string					defaultError;
	std::vector<Method::method>	acceptedMethods;
};
