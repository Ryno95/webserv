#pragma once

#include <stdexcept>
#include <string>
#include <cstring>

#include <Utility.hpp>
#include <HttpStatusCode.hpp>

namespace Webserver
{
	struct FileNotFoundException : std::runtime_error
	{
		FileNotFoundException(const std::string& file) : std::runtime_error("File could not be opened: " + file) {}
	};

	struct ParseException : std::runtime_error
	{
		ParseException(const std::string& msg) : std::runtime_error("During parsing: " + msg) {}
	};

	struct ValueDoesNotExistException : std::runtime_error
	{
		ValueDoesNotExistException(const std::string& val) : std::runtime_error("Value is not contained: " + val) {}
	};

	struct InvalidValueException : std::runtime_error
	{
		InvalidValueException(const std::string& val) : std::runtime_error("Invalid value supplied: " + val) {}
	};

	struct SystemCallFailedException : std::runtime_error
	{
		SystemCallFailedException(const std::string& call) : std::runtime_error("System call '" + call + "' failed: " + std::strerror(errno)) {}
	};

	struct InvalidRequestException : std::runtime_error
	{
		InvalidRequestException(HttpStatusCode status) : std::runtime_error("Invalid request received. Status: " + status.second), _status(status) {}
		~InvalidRequestException() throw() {}

		HttpStatusCode getStatus() const
		{
			return _status;
		}

	private:
		HttpStatusCode _status;
	};

}

namespace ConfigFileParser
{
	using namespace Webserver;

	struct ConfigParseUnexpectedTokenException : ParseException
	{
		ConfigParseUnexpectedTokenException(const std::string& expected, int lineNum, const std::string& line) : ParseException("Expected '" + expected + "' on line [" + toString(lineNum) + "]: " + line) {}
	};

	struct ConfigParseException : ParseException
	{
		ConfigParseException(int lineNum, const std::string& line) : ParseException("Failed to parse variable at line [" + toString(lineNum) + "]: " + line) {}
	};
}

