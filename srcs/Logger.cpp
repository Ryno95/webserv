#include <Logger.hpp>
#include <Colors.hpp>

#include <ctime>

std::ofstream Logger::_logFile = std::ofstream(LOGFILE);

static std::string getTimeStamp()
{
	std::string timestamp;
	struct tm* timeinfo;
	time_t rawtime;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timestamp = "[";
	timestamp += asctime(timeinfo);
	timestamp.pop_back();
	timestamp += "] ";
	return timestamp;
}

static void printWarning(const std::string& msg)
{
	std::cout << C_YELLOW << "[WARNING] " << C_RESET << msg << std::endl;
}

static void printDebug(const std::string& msg)
{
	std::cout << C_CYAN << "[DEBUG] " << C_RESET << msg << std::endl;
}



void Logger::log(const std::string& msg)
{
	if (!_logFile.is_open())
	{
		printWarning("There's no logfile, no logs will be saved.");
		return;
	}

	std::string timestamp = getTimeStamp();

	_logFile.write(timestamp.c_str(), timestamp.size());
	_logFile.write(msg.c_str(), msg.size());
	_logFile.write("\n", 1);
}

void Logger::warn(const std::string& msg)
{
	printWarning(msg);
	log("[WARNING] " + msg);
}

void Logger::debug(const std::string& msg)
{
	if (!_debugEnabled)
		return;
	printDebug(msg);

	if (_debugLoggingEnabled)
		log("[DEBUG] " + msg);
}

int main()
{
	Logger::debug("Hello world!");
	Logger::warn("THIS IS GOOOOD");
}