#pragma once

#include <string>
#include <vector>

#include <Method.hpp>

class HostConfig
{
private:
	std::string _name;
	std::string _root;
	std::string _defaultIndex;
	std::string _defaultError;
	std::vector<Method::method> _acceptedMethods;

};
