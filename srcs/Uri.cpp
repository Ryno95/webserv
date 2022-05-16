#include <Uri.hpp>

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

	void Uri::parse()
	{
		size_t pos = _raw.find(':');
		// if (pos != std::string::npos)
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
