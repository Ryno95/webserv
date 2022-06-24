#pragma once

#include <string>

namespace Webserver
{
	/*
		URI class

		Pattern:
			URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
			scheme://authority/path

		Absolute paths MUST be handled even though clients will only send them to proxies.
		Absolute:
			GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
		
		Relative:
			GET /pub/WWW/TheProject.html HTTP/1.1

		scheme		= http, https, ftp, irc, mailto, file, data
		authority	= [userinfo "@"] host [":" port]

		Example:
			http://127.0.0.1/homepage.html

		If Request-URI is an absoluteURI, the host is part of the Request-URI. Any Host header field value in the request MUST be ignored.
	*/
	class Uri
	{
	public:
		Uri();
		Uri(const std::string& uri);
		Uri(const Uri& ref);
		~Uri();

		const std::string& getResourcePath() const;
		const std::string& getHost() const;

		bool isAbsolute() const;

	private:
		void format();
		void parse();
		void parseAbsolute();

		std::string _raw;
		std::string _host; // also named authority
		std::string _path;

	};
}
