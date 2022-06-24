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
	public:
		Response();
		Response(const Response &ref);
		Response(HttpStatusCode code);
		Response &operator=(const Response &rhs);
		virtual ~Response();

		std::istream *getBodyStream() const;

		void			addFile(const std::string& filePath);
		std::istream	*getHeaderStream();
		HttpStatusCode	getStatusCode() const;
		void			setStatusCode(HttpStatusCode code);

	protected:
		void setBodyStream(std::istream* fileStream);

		HttpStatusCode		_statusCode;

	private:
		void createBodyHeaders(const std::string &fileName);
		void addConstantHeaderFields();

		std::istream*		_bodyStream;
		std::stringstream	_headerStream;
	};
}
