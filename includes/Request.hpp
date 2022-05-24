#pragma once

#include <string>
#include <map>

#include <defines.hpp>
#include <HttpStatusCode.hpp>
#include <Method.hpp>
#include <Uri.hpp>
#include <Utility.hpp>
#include <HeaderFields.hpp>

namespace Webserver
{
	class Request : public HeaderFields
	{
	#define COLON 			":"
	#define CRLF			"\r\n"
	#define CRLF_CHAR_COUNT	2
	#define TERMINATOR_LEN	4

	public:
		Request();
		Request(std::string query);
		~Request();

		void			parse();
		bool 			hasBodyField() const;

		HttpStatusCode		getStatus() const;
		Method::method 		getMethod() const;
		const std::string&	getTarget() const;
		std::string			getHost() const;
		size_t 				getBodySize() const;
		const std::string	&getBody() const;

		void				setStatus(HttpStatusCode status);

		void				appendBody(const std::string &body);

	private:
		size_t 	parseRequestLine();
		void 	parseHeaderFields(size_t pos);
		void 	parseKeyValuePair(const std::string &src, size_t newLinePos);

		size_t parseRequestMethod();
		size_t parseUri(size_t pos);
		size_t parseVersion(size_t pos);
		void validate() const;

		std::string	_query;

		Method::method		_method;
		Uri _uri;
		std::string	_version;
		std::string	_body;

		HttpStatusCode _status;

	};
}
