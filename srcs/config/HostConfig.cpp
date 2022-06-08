#include <config/HostConfig.hpp>

namespace Webserver
{
	HostConfig::HostConfig() :
		_autoIndexEnabled(false),
		_root("root"),
		_defaultIndex("index.html"),
		_defaultError("error.html"),
		_acceptedMethods(Method::GET)
	{
		_names.push_back("default");
	}

	HostConfig::HostConfig(const HostConfig& ref)
	{
		*this = ref;
	}

	HostConfig::~HostConfig()
	{
	}

	HostConfig& HostConfig::operator=(const HostConfig& ref)
	{
		_names = ref._names;
		_children = ref._children;
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

	const std::vector<LocationConfig*>& HostConfig::getLocationConfigs() const
	{
		return _children;
	}

	bool HostConfig::isAutoIndexEnabled() const
	{
		return _autoIndexEnabled;
	}

	const std::string& HostConfig::getRoot() const
	{
		return _root;
	}

	const std::string& HostConfig::getDefaultIndex() const
	{
		return _defaultIndex;
	}

	const std::string& HostConfig::getDefaultError() const
	{
		return _defaultError;
	}

	const std::vector<Method::method> HostConfig::getAcceptedMethods() const
	{
		return _acceptedMethods;
	}
}
