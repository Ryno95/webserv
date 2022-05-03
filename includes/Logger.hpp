#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define ENABLE_DEBUGGING		true
#define ENABLE_DEBUG_LOGGING	true
#define ENABLE_LOGGING			true
#define LOGFILE					"logs.log"

#define DEBUG(x) Logger::inputStream << x; Logger::debug()
#define WARN(x) Logger::inputStream << x; Logger::warn()
#define ERROR(x) Logger::inputStream << x; Logger::error()

class Logger
{
public:
	static void debug();
	static void warn();
	static void error();

	static std::stringstream inputStream;

private:
	static std::ofstream _logFile;

	static void log(const std::string& msg);
};
