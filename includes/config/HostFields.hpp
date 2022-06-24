#pragma once

#include <string>
#include <vector>
#include <map>

#include <Method.hpp>
#include <HttpStatusCode.hpp>

namespace Webserver
{
	class HostFields
	{
	public:

	typedef std::pair<unsigned int, std::string>	ErrorPage;
	typedef std::map<unsigned int, std::string>		ErrorPages;

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
		const ErrorPages& getErrorPages() const;

	protected:
		bool						_autoIndexEnabled;
		std::string					_root;
		std::string					_defaultIndex;
		std::string					_defaultError;
		std::vector<Method::method>	_acceptedMethods;
		bool						_allowUpload;
		ErrorPages					_errorPages;
	};
}
