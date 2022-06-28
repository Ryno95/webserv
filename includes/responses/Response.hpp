#pragma once

#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include <HttpStatusCode.hpp>
#include <defines.hpp>
#include <Utility.hpp>
#include <HeaderFields.hpp>
#include <SendStream.hpp>

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

		SendStream *getBodyStream() const;

		void			addFile(const std::string& filePath);
		SendStream	*getHeaderStream();
		HttpStatusCode	getStatusCode() const;
		void			setStatusCode(HttpStatusCode code);

		bool			isFinished() const;
		void			setFinished();
		void			setNotFinished();

		void setBodyStream(SendStream* fileStream);
	protected:

		HttpStatusCode		_statusCode;

	private:
		void createBodyHeaders(const std::string &fileName);
		void addConstantHeaderFields();

		SendStream*	_bodyStream;
		SendStream	_headerStream;
		bool		_isFinished;
	};
}
