#include <config/HostConfig.hpp>

HostConfig::HostConfig() 
:	AConfig::AConfig(fillVariablesMap()),
	autoIndexEnabled(false),
	root("root"),
	defaultIndex("index.html"),
	defaultError("error.html"),
	acceptedMethods(Method::GET)
{
	names.push_back("default");
}

HostConfig::HostConfig(const HostConfig& ref) : AConfig::AConfig(fillVariablesMap())
{
	*this = ref;
}

HostConfig::~HostConfig()
{
}

HostConfig& HostConfig::operator=(const HostConfig& ref)
{
	names = ref.names;
	locations = ref.locations;
	autoIndexEnabled = ref.autoIndexEnabled;
	root = ref.root;
	defaultIndex = ref.defaultIndex;
	defaultError = ref.defaultError;
	acceptedMethods = ref.acceptedMethods;
	return *this;
}

AConfig::map_type HostConfig::fillVariablesMap()
{
	map_type map;
	map["name"]				= var_data(var_strings, &names);
	map["root"]				= var_data(var_string, &root);
	map["methods"]			= var_data(var_methods, &acceptedMethods);
	map["default_index"]	= var_data(var_string, &defaultIndex);
	map["default_error"]	= var_data(var_string, &defaultError);
	map["location"]			= var_data(var_strings, &locations);
	map["autoindex"]		= var_data(var_bool, &autoIndexEnabled);
	return map;
}
