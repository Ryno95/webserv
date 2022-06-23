#pragma once

#include <string>
#include <vector>

#include <Method.hpp>

namespace Webserver
{
	class HostFields
	{
	public:
		HostFields();
		~HostFields();
		HostFields(const HostFields& ref);
		HostFields& operator=(const HostFields& ref);

		bool isAutoIndexEnabled() const;
		const std::string& getRoot() const;
		const std::string& getDefaultIndex() const;
		const std::string& getDefaultError() const;
		const std::vector<Method::method>& getAcceptedMethods() const;
		bool isUploadAllowed() const;

	protected:
		bool						_autoIndexEnabled;
		std::string					_root;
		std::string					_defaultIndex;
		std::string					_defaultError;
		std::vector<Method::method>	_acceptedMethods;
		bool						_allowUpload;
	};
}
