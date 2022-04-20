#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define ENABLE_DEBUGGING		true
#define ENABLE_DEBUG_LOGGING	true
#define ENABLE_LOGGING			true
#define LOGFILE					"warnings.log"

#define DEBUG(x) { std::stringstream stream; Logger::debug(stream << x); }
#define WARN(x) { std::stringstream stream; Logger::warn(stream << x); }

// #define DEBUG(x) Logger::debug(std::stringstream() << x)
// #define WARN(x) Logger::warn(std::stringstream() << x)

class Logger
{
public:
	static void debug(const std::string& msg);
	static void debug(const std::ostream& ss);
	static void warn(const std::string& msg);
	static void warn(const std::ostream& ss);

private:
	static std::ofstream _logFile;

	static void log(const std::string& msg);
};
