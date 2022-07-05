#pragma once

#include <string>
#include <map>

#include <defines.hpp>
#include <HttpStatusCode.hpp>
#include <Method.hpp>
#include <Uri.hpp>
#include <Utility.hpp>
#include <HeaderFields.hpp>

#define COLON 			":"
#define CRLF			"\r\n"
#define CRLF_CHAR_COUNT	2
#define TERMINATOR_LEN	4

namespace Webserver
{
	class Request : public HeaderFields
	{
	public:
		Request();
		~Request();

		HttpStatusCode		getStatus() const;
		Method::method 		getMethod() const;
		const std::string&	getTarget() const;
		std::string			getHost() const;
		size_t 				getBodySize() const;
		const std::string&	getBody() const;

		void				setStatus(HttpStatusCode status);

		void				appendBody(const std::string &body);

		Uri 				_uri;

	protected:
		std::string			_query;

		Method::method		_method;
		std::string			_version;
		std::string			_body;
		HttpStatusCode		_status;
		

	};
}
