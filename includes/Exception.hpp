#pragma once

#include <stdexcept>
#include <string>
#include <cstring>

#include <Utility.hpp>

struct FileNotFoundException : std::runtime_error
{
	FileNotFoundException(const std::string& file) : std::runtime_error("File could not be opened: " + file) {}
};

struct ParseException : std::runtime_error
{
	ParseException(const std::string& msg) : std::runtime_error("During parsing: " + msg) {}
};

struct ConfigParseException : ParseException
{
	ConfigParseException(int lineNum, const std::string& line) : ParseException("Failed to parse variable at line [" + Util::toString(lineNum) + "]: " + line) {}
};

struct ConfigParseUnexpectedTokenException : ParseException
{
	ConfigParseUnexpectedTokenException(const std::string& expected, int lineNum, const std::string& line) : ParseException("Expected '" + expected + "' on line [" + Util::toString(lineNum) + "]: " + line) {}
};

struct NotContainedException : std::runtime_error
{
	NotContainedException(const std::string& val) : std::runtime_error("Value is not contained: " + val) {}
};

struct InvalidValueException : std::runtime_error
{
	InvalidValueException(const std::string& val) : std::runtime_error("Invalid value supplied: " + val) {}
};

struct SystemCallFailedException : std::runtime_error
{
	SystemCallFailedException(const std::string& call) : std::runtime_error("System call '" + call + "' failed: " + std::strerror(errno)) {}
};
