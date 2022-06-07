#include <config/HostConfig.hpp>

namespace Webserver
{
	HostConfig::HostConfig() :
		_autoIndexEnabled(false),
		_root("root"),
		_defaultIndex("index.html"),
		_defaultError("error.html"),
		_acceptedMethods(Method::GET)
	{
		_names.push_back("default");
	}

	HostConfig::HostConfig(const HostConfig& ref)
	{
		*this = ref;
	}

	HostConfig::~HostConfig()
	{
	}

	HostConfig& HostConfig::operator=(const HostConfig& ref)
	{
		_names = ref._names;
		_locations = ref._locations;
		_autoIndexEnabled = ref._autoIndexEnabled;
		_root = ref._root;
		_defaultIndex = ref._defaultIndex;
		_defaultError = ref._defaultError;
		_acceptedMethods = ref._acceptedMethods;
		return *this;
	}

	// AConfig::map_type HostConfig::fillVariablesMap()
	// {
	// 	map_type map;
	// 	map["name"]				= var_data(var_strings, &names);
	// 	map["root"]				= var_data(var_string, &root);
	// 	map["methods"]			= var_data(var_methods, &acceptedMethods);
	// 	map["default_index"]	= var_data(var_string, &defaultIndex);
	// 	map["default_error"]	= var_data(var_string, &defaultError);
	// 	map["autoindex"]		= var_data(var_bool, &autoIndexEnabled);
	// 	return map;
	// }

	// void HostConfig::validate() const
	// {
	// 	return ;
	// }
}
