#pragma once

#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include <HttpStatusCode.hpp>
#include <defines.hpp>
#include <Utility.hpp>
#include <HeaderFields.hpp>

namespace Webserver
{
	#define NotFoundErrorPage "root/NotFoundErrorPage.html"
	#define BadRequestErrorPage "root/BadRequestErrorPage.html"

	class Response : public HeaderFields
	{
	protected:
		Response();
		Response(const Response &ref);
		Response(HttpStatusCode code);
		Response &operator=(const Response &rhs);

	public:
		virtual ~Response();

		void	setStatusCode(HttpStatusCode code);
		void	setBodyStream(std::ifstream* stream);

		void				buildHeader();
		std::stringstream	*getHeaderStream();
		// void				addHeaderField(const std::string& key, const std::string& value);
		void				createContentHeaders(const std::string &fileName);

		std::ifstream		*getBodyStream();
		HttpStatusCode		getStatusCode() const;
		const HeaderFields& getHeaders() const;

	protected:

		HttpStatusCode 						_statusCode;
		std::stringstream 					_headerStream;
		std::ifstream						*_bodyStream;
		// std::map<std::string, std::string, CmpCaseInsensitive>	_headerFields;
		// HeaderFields						_headers;

		void				addConstantHeaderFields(); // hard coded for now to get the flow going
	
	};
}
