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

// const std::string Content-MD5 121
// const std::string Content-Range 122
// const std::string Content-Type 124
// const std::string Date 124
// const std::string Clockless Origin Server Operation 125
// const std::string ETag 126
// const std::string Expect 126
// const std::string Expires 127
// const std::string From 128
// const std::string Host 128
// const std::string If-Match 129
// const std::string If-Modified-Since 130
// const std::string If-None-Match 132
// const std::string If-Range 133
// const std::string If-Unmodified-Since 134
// const std::string Last-Modified 134
// const std::string Location 135
// const std::string Max-Forwards 136
// const std::string Pragma 136
// const std::string Proxy-Authenticate 137
// const std::string Proxy-Authorization 137
// const std::string Range 138
// const std::string Byte Ranges 138
// const std::string Range Retrieval Requests 139
// const std::string Referer 140
// const std::string Retry-After 141
// const std::string Server 141
// const std::string TE 142
// const std::string Trailer 143
// const std::string Transfer-Encoding 143
// const std::string Upgrade 144
// const std::string User-Agent 145
// const std::string Vary 145
// const std::string Via 146
// const std::string Warning 148
// const std::string WWW-Authenticate 150
}

namespace Webserver
{
	class HeaderFields
	{
	public:
		bool containsHeader(const std::string& key) const;
		void addHeader(const std::string& key, const std::string& value);
		bool tryGetHeader(const std::string& key, std::string& value) const;
		// const std::string& operator[](const std::string& key);
	
	private:
		std::map<std::string, std::string, CmpCaseInsensitive> _map;

	};
}
