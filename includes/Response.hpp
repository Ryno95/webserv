#pragma once

#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include <HttpStatusCode.hpp>
#include <Request.hpp>

namespace Webserver
{
	#define NotFoundErrorPage "root/BadRequestErrorPage.html"

	class Response
	{
	public:
		Response();
		Response(const Response &ref);
		Response(HttpStatusCode code);
		~Response();

		void	setStatusCode(HttpStatusCode code);
		void	setBodyStream(std::ifstream* stream);

		Response			&operator=(const Response &rhs);

		void				buildHeader();
		std::stringstream	*getHeaderStream();
		void				addHeaderFields(); // hard coded for now to get the flow going
		void				addHeaderField(std::string key, std::string value);
		void				createContentHeaders(const std::string &fileName);

		std::ifstream		*getBodyStream();
		HttpStatusCode		getStatusCode() const;

	protected:

		HttpStatusCode 						_statusCode;
		std::stringstream 					_headerStream;
		std::ifstream						*_bodyStream;
		std::map<std::string, std::string>	_headerFields;
	};
}
