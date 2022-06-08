#include <config/ParseTreeUtility.hpp>
#include <config/AConfigParser.hpp>

namespace Webserver
{
	BeginCommand::BeginCommand(AConfigParser* instance) : _instance(instance)
	{
	}

	BeginCommand::~BeginCommand()
	{
	}

	void BeginCommand::callback(const std::string& args)
	{
		(void)args;
		_instance->begin();
	}



	EndCommand::EndCommand(AConfigParser* instance) : _instance(instance)
	{
	}

	EndCommand::~EndCommand()
	{
	}

	void EndCommand::callback(const std::string& args)
	{
		(void)args;
		_instance->end();
	}
}
