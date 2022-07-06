#include <config/HostConfig.hpp>

namespace Webserver
{
	HostConfig::HostConfig() : HostFields::HostFields()
	{
		_names.push_back("default");
	}

	HostConfig::HostConfig(const HostConfig& ref) : HostFields::HostFields(ref)
	{
		*this = ref;
	}

	HostConfig::~HostConfig()
	{
	}

	HostConfig& HostConfig::operator=(const HostConfig& ref)
	{
		_names = ref._names;
		_autoIndexEnabled = ref._autoIndexEnabled;
		_root = ref._root;
		_defaultIndex = ref._defaultIndex;
		_defaultError = ref._defaultError;
		_acceptedMethods = ref._acceptedMethods;
		return *this;
	}

	const std::vector<std::string>& HostConfig::getHostNames() const
	{
		return _names;
	}
}
