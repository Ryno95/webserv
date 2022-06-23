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

	class AResponse : public HeaderFields
	{
	protected:
		AResponse();
		AResponse(const AResponse &ref);
		AResponse(HttpStatusCode code);
		AResponse &operator=(const AResponse &rhs);

	public:
		virtual std::istream *getBodyStream() = 0;
		virtual ~AResponse();

		void	setStatusCode(HttpStatusCode code);

		void				buildHeader();
		std::istream		*getHeaderStream();
		void				createContentHeaders(const std::string &fileName);

		HttpStatusCode		getStatusCode() const;
		const HeaderFields& getHeaders() const;


	protected:

		HttpStatusCode		_statusCode;
		std::stringstream	_headerStream;

		void				addConstantHeaderFields(); // hard coded for now to get the flow going
	
	};
}
