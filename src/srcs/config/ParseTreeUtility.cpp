#include <config/ParseTreeUtility.hpp>
#include <config/ATopDownParser.hpp>

namespace Webserver
{
	BeginCommand::BeginCommand(ATopDownParser* instance) : _instance(instance)
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



	EndCommand::EndCommand(ATopDownParser* instance) : _instance(instance)
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
