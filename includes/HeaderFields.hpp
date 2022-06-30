#pragma once

#include <string>
#include <map>

#include <Utility.hpp>

namespace Header
{
	const std::string Accept = "Accept";
	const std::string AcceptCharset = "Accept-Charset";
	const std::string AcceptEncoding = "Accept-Encoding";
	const std::string AcceptLanguage = "Accept-Language";
	const std::string AcceptRanges = "Accept-Ranges";
	const std::string Age = "Age";
	const std::string Allow = "Allow";
	const std::string Authorization = "Authorization";
	const std::string CacheControl = "Cache-Control";
	const std::string WhatisCacheable = "What is Cacheable";
	const std::string WhatMaybeStoredbyCaches  = "What May be Stored by Caches";
	const std::string ModificationsoftheBasicExpirationMechanism = "Modifications of the Basic Expiration Mechanism";
	const std::string CacheRevalidationandReloadControls = "Cache Revalidation and Reload Controls";
	const std::string NoTransformDirective = "No-Transform Directive";
	const std::string CacheControlExtensions = "Cache Control Extensions";
	const std::string Connection = "Connection";
	const std::string ContentEncoding = "Content-Encoding";
	const std::string ContentLanguage = "Content-Language";
	const std::string ContentLength = "Content-Length";
	const std::string ContentLocation = "Content-Location";
	const std::string ContentMD5 = "Content-MD5";
	const std::string ContentRange = "Content-Range";
	const std::string ContentType = "Content-Type";
	const std::string Date = "Date";
	const std::string ClocklessOriginServerOperation = "Clockless Origin Server Operation";
	const std::string ETag = "ETag";
	const std::string Expect = "Expect";
	const std::string Expires = "Expires";
	const std::string From = "From";
	const std::string Host = "Host";
	const std::string IfMatch = "If-Match";
	const std::string IfModifiedSince = "If-Modified-Since";
	const std::string IfNoneMatch = "If-None-Match";
	const std::string IfRange = "If-Range";
	const std::string IfUnmodifiedSince = "If-Unmodified-Since";
	const std::string LastModified = "Last-Modified";
	const std::string Location = "Location";
	const std::string MaxForwards = "Max-Forwards";
	const std::string Pragma = "Pragma";
	const std::string ProxyAuthenticate = "Proxy-Authenticate";
	const std::string ProxyAuthorization = "Proxy-Authorization";
	const std::string Range = "Range";
	const std::string ByteRanges = "Byte Ranges";
	const std::string RangeRetrievalRequests = "Range Retrieval Requests";
	const std::string Referer = "Referer";
	const std::string RetryAfter = "Retry-After";
	const std::string Server = "Server";
	const std::string TE = "TE";
	const std::string Trailer = "Trailer";
	const std::string TransferEncoding = "Transfer-Encoding";
	const std::string Upgrade = "Upgrade";
	const std::string UserAgent = "User-Agent";
	const std::string Vary = "Vary";
	const std::string Via = "Via";
	const std::string Warning = "Warning";
	const std::string WWWAuthenticate = "WWW-Authenticate";

	const std::string SetCookie = "Set-Cookie";

	/*
		Cgi specific headers
	*/
	const std::string Status = "Status";
}

namespace Webserver
{
	class HeaderFields
	{
	public:
		typedef std::map<std::string, std::string, CmpCaseInsensitive>	map_type;
		typedef map_type::value_type									pair_type;
		typedef map_type::const_iterator								const_iterator;

		bool containsHeader(const std::string& key) const;
		void addHeader(const std::string& key, const std::string& value);
		bool tryGetHeader(const std::string& key, std::string& value) const;
		const_iterator headersBegin() const;
		const_iterator headersEnd() const;

	protected:
		void setHeaderFields(const HeaderFields& ref);

	private:
		map_type _map;

	};
}
