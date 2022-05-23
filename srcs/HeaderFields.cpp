#include <HeaderFields.hpp>

namespace Webserver
{
	bool HeaderFields::containsHeader(const std::string& key) const
	{
		return _map.find(key) != _map.end();
	}

	/*
		If key exists in the map, this function sets parameter value to the mapped value type
		and returns true.
		Otherwise value remains unchanged and false is returned.
	*/
	bool HeaderFields::tryGetHeader(const std::string& key, std::string& value) const
	{
		std::map<std::string, std::string>::iterator pos = _map.find(key);
		if (pos == _map.end())
			return false;
		value = pos->second;
		return true;
	}

	void HeaderFields::addHeader(const std::string& key, const std::string& value)
	{
		_map.insert(std::pair<std::string, std::string>(key, value));
	}

	// const std::string& HeaderFields::operator[](const std::string& key)
	// {
	// 	return _map[key];
	// }
}
