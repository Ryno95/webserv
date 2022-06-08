#pragma once
#include <config/AConfigParser.hpp>
#include <config/AppConfig.hpp>
#include <config/ServerConfigParser.hpp>

namespace Webserver
{
	class ConfigParser : public AConfigParser, public IBranch<AppConfig, ServerConfigParser>
	{
	public:
		typedef AppConfig data_type;
		typedef ServerConfigParser child_type;

		ConfigParser(std::istream& stream);
		~ConfigParser();
		void parse();

		data_type* getDataStruct() const;

		virtual data_type* getData() const
		{
			return new data_type(_data);
		}

		// virtual child_type* getChildData() const
		// {
		// 	return new child_type();
		// }

	private:
		std::map<std::string, ICommand*> createKeywords();
		void validate();

		data_type _data;
	};
}
