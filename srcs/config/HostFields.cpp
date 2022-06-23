#include <config/HostFields.hpp>

#include <Logger.hpp>

namespace Webserver
{
	HostFields::HostFields() :
		_autoIndexEnabled(false),
		_root("root"),
		_defaultIndex("index.html"),
		_defaultError("error.html"),
		_acceptedMethods(Method::GET),
		_allowUpload(false)
	{
	}

	HostFields::HostFields(const HostFields& ref)
	{
		*this = ref;
	}

	HostFields& HostFields::operator=(const HostFields& ref)
	{
		_autoIndexEnabled = ref._autoIndexEnabled;
		_root = ref._root;
		_defaultIndex = ref._defaultIndex;
		_defaultError = ref._defaultError;
		_acceptedMethods = ref._acceptedMethods;
		_allowUpload = ref._allowUpload;
		return *this;
	}

	HostFields::~HostFields()
	{
	}

	bool HostFields::isAutoIndexEnabled() const
	{
		return _autoIndexEnabled;
	}

	const std::string& HostFields::getRoot() const
	{
		return _root;
	}

	const std::string& HostFields::getDefaultIndex() const
	{
		return _defaultIndex;
	}

	const std::string& HostFields::getDefaultError() const
	{
		return _defaultError;
	}

	const std::vector<Method::method>& HostFields::getAcceptedMethods() const
	{
		return _acceptedMethods;
	}

	bool HostFields::isUploadAllowed() const
	{
		return _allowUpload;
	}
}
