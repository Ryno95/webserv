#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define DEBUGGING		true
#define LOG_DEBUGGING	true
#define LOGFILE			"warnings.log"

class Logger
{
public:
	static void debug(const std::string& msg);
	static void warn(const std::string& msg);

private:
	static const bool _debugEnabled = DEBUGGING;
	static const bool _debugLoggingEnabled = LOG_DEBUGGING;
	static std::ofstream _logFile;

	static void log(const std::string& msg);
};
