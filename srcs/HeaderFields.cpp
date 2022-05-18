#include <HeaderFields.hpp>
#include <Utility.hpp>

namespace Webserver
{
	HeaderFields::HeaderFields()
	{
	}

	HeaderFields::~HeaderFields()
	{

	}

	std::string HeaderFields::operator[](const std::string& key)
	{
		return _map[stringToLower(key)];
	}

	void HeaderFields::add(const std::string& key, const std::string& value)
	{
		_map[stringToLower(key)] = value;
	}
}
