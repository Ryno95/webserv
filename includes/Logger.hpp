#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define DEBUG_ENABLED		true
#define LOGGING_ENABLED		true
#define LOGFILE				"logs.log"

#define DEBUG(x) Logger::inputStream << x; Logger::debug()
#define WARN(x) Logger::inputStream << x; Logger::warn()
#define ERROR(x) Logger::inputStream << x; Logger::error()

namespace Webserver
{
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
}
