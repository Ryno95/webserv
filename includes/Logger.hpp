#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define ENABLE_DEBUGGING		true
#define ENABLE_DEBUG_LOGGING	true
#define ENABLE_LOGGING			true
#define LOGFILE					"warnings.log"

#define DEBUG(x) Logger::inputStream << x; Logger::debug()
#define WARN(x) Logger::inputStream << x; Logger::warn()

class Logger
{
public:
	static void debug();
	static void warn();

	static std::stringstream inputStream;

private:
	static std::ofstream _logFile;

	static void log(const std::string& msg);
};
