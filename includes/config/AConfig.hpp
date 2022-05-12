#pragma once

#include <map>
#include <vector>
#include <cstdlib>

#include <Utility.hpp>
#include <Method.hpp>
#include <Logger.hpp>
#include <Exception.hpp>

namespace Webserver
{
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
		virtual void	 validate() const = 0;

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
			throw InvalidValueException(value);
		return std::atoi(value.c_str());
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
			throw InvalidValueException(value);
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
			str = removeLeadingWhitespace(str);
		}
		return values;
	}

	template<>
	inline std::vector<Method::method> AConfig::parse(const std::string& value) const
	{
		std::string str = value;
		std::string methodStr;
		std::vector<Method::method> values;

		while (true)
		{
			size_t end = std::min(std::min(str.find(' '), str.find('\t')), str.size());

			Method::method methodId = parseMethod(str.substr(0, end));
			if (methodId == Method::INVALID)
				throw InvalidValueException(value);

			values.push_back(methodId);

			if (end == str.size())
				break;

			str = str.substr(end, str.size() - end);
			str = removeLeadingWhitespace(str);
		}
		return values;
	}
}
