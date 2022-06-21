#include <algorithm>

#include <Uri.hpp>
#include <Utility.hpp>
#include <Exception.hpp>
#include <Logger.hpp>

namespace Webserver
{
	Uri::Uri() {}

	Uri::Uri(const std::string& uri) : _raw(uri)
	{
		parse();
		format();
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

	void Uri::format()
	{
		// Make sure every uri starts with '/'
		if (_path.size() == 0 || _path[0] != '/')
			_path.insert(0, "/");

		// Replace double forward slashes by single forward slashes
		while (true)
		{
			size_t pos = _path.find("//");
			if (pos == std::string::npos)
				break;

			_path.erase(_path.begin() + pos);
		}

		// Remove '/.' from '/./' occurences
		while (true)
		{
			size_t pos = _path.find("/./");
			if (pos == std::string::npos)
				break;

			_path.erase(_path.begin() + pos, _path.begin() + pos + 2);
		}

		// Remove '/..' and the (if any) preceding directory
		while (true)
		{
			size_t pos = _path.find("/../");
			if (pos == std::string::npos)
				break;

			if (pos == 0)
				_path.erase(_path.begin(), _path.begin() + 3);
			else
				_path.erase(_path.begin() + _path.rfind("/", pos - 1), _path.begin() + pos + 3);
		}

		// Remove trailing '/.'
		{
			if (_path.find("/.", _path.size() - 2) != std::string::npos)
				_path.erase(_path.end() - 1, _path.end());
		}

		// Remove trailing '/..'
		{
			if (_path.find("/..", _path.size() - 3) != std::string::npos)
				_path.erase(_path.end() - 2, _path.end());
		}

		// Make sure we have at least '/' left
		if (_path.size() == 0)
			_path = "/";
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
		setPath(_raw.substr(end, std::string::npos));
	}

	void Uri::setPath(const std::string &path)
	{
		_path = path;
	}

	void Uri::parse()
	{
		// parse host if it's an absolute uri (determined with ':')
		if (_raw.find(':') != std::string::npos)
			parseAbsolute();
		else
			_path = _raw;
	}

	const std::string& Uri::getResourcePath() const
	{
		return _path;
	}

	const std::string& Uri::getHost() const
	{
		return _host;
	}
}
