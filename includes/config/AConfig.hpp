#pragma once

#include <map>
#include <vector>

#include <Utility.hpp>
#include <Method.hpp>
#include <Logger.hpp>

class AConfig
{
public:
	void parseVariable(const std::string& varName, const std::string& value);

protected:
	enum types
	{
		var_uint,
		var_bool,
		var_string,
		var_strings,
		var_methods
	};

	typedef std::pair<int, void*>			var_data;
	typedef std::map<std::string, var_data>	map_type;

	AConfig(const map_type& map);
	virtual ~AConfig();

	virtual map_type fillVariablesMap() = 0;

	template<class T>
	T parse(const std::string& value) const;


private:
	const map_type _vars;
	map_type::const_iterator getElement(const std::string& varName);


};



template<>
inline uint AConfig::parse(const std::string& value) const
{
	if (value.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Invalid value");
	return atoi(value.c_str());
}

template<>
inline std::string AConfig::parse(const std::string& value) const
{
	return value;
}

template<>
inline bool AConfig::parse(const std::string& value) const
{
	if (value == "true")
		return true;
	else if (value == "false")
		return false;
	else
		throw std::runtime_error("Invalid value");
}

template<>
inline std::vector<std::string> AConfig::parse(const std::string& value) const
{
	std::string str = value;
	std::vector<std::string> values;

	while (true)
	{
		size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());
		values.push_back(str.substr(0, end));

		if (end == str.size())
			break;

		str = str.substr(end, str.size() - end);
		str = Util::removeLeadingWhitespace(str);
	}
	return values;
}

template<>
inline std::vector<Method::method> AConfig::parse(const std::string& value) const
{
	std::string str = value;
	std::string method;
	std::vector<Method::method> values;

	while (true)
	{
		size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());
		method = str.substr(0, end);

		if (method == "GET")
			values.push_back(Method::GET);
		else if (method == "POST")
			values.push_back(Method::POST);
		else if (method == "DELETE")
			values.push_back(Method::DELETE);
		else
		{
			ERROR("Unhandled method supplied: " << method);
			throw std::runtime_error("Unhandled method supplied");
		}

		if (end == str.size())
			break;

		str = str.substr(end, str.size() - end);
		str = Util::removeLeadingWhitespace(str);
	}
	return values;
}
