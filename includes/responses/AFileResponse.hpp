#pragma once

#include <fstream>

#include <responses/AResponse.hpp>
#include <HeaderFields.hpp>

namespace Webserver
{
	class AFileResponse : public AResponse
	{
	protected:
		AFileResponse();
		AFileResponse(HttpStatusCode code);

		virtual ~AFileResponse();

		void setFileStream(std::ifstream* fileStream);
		void openFile(const std::string& filePath);

	public:
		std::istream *getBodyStream();
		void createContentHeaders(const std::string& fileName);

	private:
		std::ifstream *_fileStream;
	};
}
