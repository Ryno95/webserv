#include <Uri.hpp>
#include <Utility.hpp>
#include <Exception.hpp>

namespace Webserver
{
	Uri::Uri() {}

	Uri::Uri(const std::string& uri) : _raw(uri)
	{
		parse();
	}

	Uri::Uri(const Uri& ref)
	{
		_raw = ref._raw;
		_host = ref._host;
		_path = ref._path;
	}

	Uri::~Uri()
	{
	}

	bool Uri::isAbsolute() const
	{
		return _host.size() != 0;
	}

	void Uri::normalize()
	{
	}

	void Uri::parseAbsolute()
	{
		static const size_t schemeLen = 7; // [http://]
		// For absolute uri, we need AT LEAST:
		// [http://] [/] = http:/// = 8 bytes

		if (_raw.size() <= schemeLen)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		if (stringToLower(_raw.substr(0, schemeLen)) != "http://")
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		size_t end = _raw.find('/', schemeLen);
		if (end == std::string::npos)
			throw InvalidRequestException(HttpStatusCodes::BAD_REQUEST);

		_host = _raw.substr(schemeLen, end - schemeLen);
		_path = _raw.substr(end, std::string::npos);
	}

	void Uri::parse()
	{
		// parse host if it's an absolute uri (determined with ':')
		if (_raw.find(':') != std::string::npos)
			parseAbsolute();
		else
			_path = _raw;
	}

	std::string Uri::getResourcePath() const
	{
		return _path;
	}

	std::string Uri::getHost() const
	{
		return _host;
	}
}
