#include <Colors.hpp>
#include <Logger.hpp>
#include <Utility.hpp>
#include <Webserv.hpp>

#include <ctime>
#include <sstream>

namespace Webserver
{
	std::ofstream Logger::_logFile;
	std::stringstream Logger::inputStream;

	static void printError(const std::string& msg)
	{
		std::cout << C_RED << "[ERROR] " << C_RESET << msg << std::endl;
	}

	static void printWarning(const std::string& msg)
	{
		std::cout << C_YELLOW << "[WARNING] " << C_RESET << msg << std::endl;
	}

	static void printDebug(const std::string& msg)
	{
		if (DEBUG_ENABLED)
			std::cout << C_CYAN << "[DEBUG] " << C_RESET << msg << std::endl;
	}

	void Logger::log(const std::string& msg)
	{
		if (!LOGGING_ENABLED)
			return;

		if (!_logFile.is_open())
		{
			_logFile.open(LOGFILE);
			if (!_logFile.is_open())
				{
					printWarning("Could not create a logfile, no logs will be saved.");
					return;
				}
		}

		std::string timestamp = "[ " + getTimeStamp() + " ] ";

		_logFile.write(timestamp.c_str(), timestamp.size());
		_logFile.write(msg.c_str(), msg.size());
		_logFile.write("\n", 1);
		_logFile.flush();
	}

	void Logger::error()
	{
		std::string str = inputStream.str();
		inputStream.str("");
		printError(str);
		log("[ERROR] " + str);
	}

	void Logger::warn()
	{
		std::string str = inputStream.str();
		inputStream.str("");
		printWarning(str);
		log("[WARNING] " + str);
	}

	void Logger::debug()
	{
		std::string str = inputStream.str();
		inputStream.str("");
		printDebug(str);
		log("[DEBUG] " + str);
	}
}
