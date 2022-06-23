#include <responses/AFileResponse.hpp>
#include <Exception.hpp>
#include <Webserv.hpp>

namespace Webserver
{
	AFileResponse::AFileResponse() : AResponse(),
		_fileStream(nullptr)
	{
	}

	AFileResponse::AFileResponse(HttpStatusCode code) : AResponse(code),
		_fileStream(nullptr)
	{
	}

	AFileResponse::~AFileResponse()
	{
	}

	void AFileResponse::openFile(const std::string& filePath)
	{
		setFileStream(new std::ifstream(filePath));

		if (!_fileStream->is_open())
			throw InvalidRequestException(HttpStatusCodes::NOT_FOUND);

		createContentHeaders(filePath); // probably not OK yet, because this is the full file path and not just the file name.
	}

	void AFileResponse::setFileStream(std::ifstream* fileStream)
	{
		if (_fileStream != nullptr)
			delete _fileStream;

		_fileStream = fileStream;
	}

	std::istream* AFileResponse::getBodyStream()
	{
		return _fileStream;
	}

	static std::string getContentTypeHeader(const std::string &fileName)
	{
		const size_t	extensionIndex = fileName.find_last_of(".");
		std::string		fileExtensionWithoutDot;
		std::string 	mimeType;

		if (extensionIndex != std::string::npos)
			fileExtensionWithoutDot = fileName.substr(extensionIndex + 1, std::string::npos);

		mimeType = Webserv::config().getMimeTypes().getMIMEType(fileExtensionWithoutDot);

		return mimeType;
	}

	void AFileResponse::createContentHeaders(const std::string &fileName)
	{
		getBodyStream()->seekg(0, std::ios_base::end);
		addHeader(Header::ContentLength, std::to_string(_fileStream->tellg()));
		addHeader(Header::ContentType, getContentTypeHeader(fileName));
	}
}
