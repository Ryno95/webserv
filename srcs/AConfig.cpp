#include <AConfig.hpp>

AConfig::AConfig()
{
}

AConfig::~AConfig()
{
}

AConfig::map_type::const_iterator AConfig::getElement(const std::string& varName)
{
	map_type::const_iterator it = _vars.find(varName);

	if (it == _vars.end())
		throw std::runtime_error("Variable not contained");
	return it;
}

void AConfig::parseVariable(const std::string& varName, const std::string& value)
{
	map_type::const_iterator it = getElement(varName);

	switch (it->second.first)
	{
		case var_uint:
			*((uint*)it->second.second) = parse<uint>(value);
			break;

		case var_bool:
			*((bool*)it->second.second) = parse<bool>(value);
			break;

		case var_string:
			*((std::string*)it->second.second) = parse<std::string>(value);
			break;

		case var_strings:
			*((std::vector<std::string>*)it->second.second) = parse<std::vector<std::string>>(value);
			break;

		case var_methods:
			*((std::vector<Method::method>*)it->second.second) = parse<std::vector<Method::method>>(value);
			break;
	}
}
