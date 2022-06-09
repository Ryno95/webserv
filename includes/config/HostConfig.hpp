#pragma once

#include <string>
#include <vector>

#include <Method.hpp>
#include <config/AComposite.hpp>

namespace Webserver
{
	class LocationConfig;

	class HostConfig : public AComposite<LocationConfig*>
	{
		friend class HostConfigParser;

	public:
		HostConfig();
		HostConfig(const HostConfig& ref);
		~HostConfig();
		HostConfig& operator=(const HostConfig& ref);

		const std::vector<std::string>& getHostNames() const;
		bool isAutoIndexEnabled() const;
		const std::string& getRoot() const;
		const std::string& getDefaultIndex() const;
		const std::string& getDefaultError() const;
		const std::vector<Method::method> getAcceptedMethods() const;

	private:
		std::vector<std::string>	_names;

	protected:
		bool						_autoIndexEnabled;
		std::string					_root;
		std::string					_defaultIndex;
		std::string					_defaultError;
		std::vector<Method::method>	_acceptedMethods;
	};
}
