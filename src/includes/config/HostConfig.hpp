#pragma once

#include <string>
#include <vector>

#include <Method.hpp>
#include <config/AComposite.hpp>
#include <config/HostFields.hpp>

namespace Webserver
{
	class LocationConfig;

	class HostConfig : public AComposite<LocationConfig*>, public HostFields
	{
		friend class HostConfigParser;

	public:
		HostConfig();
		HostConfig(const HostConfig& ref);
		~HostConfig();
		HostConfig& operator=(const HostConfig& ref);

		const std::vector<std::string>& getHostNames() const;

	private:
		std::vector<std::string>	_names;

	};
}
