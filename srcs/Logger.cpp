#include <Logger.hpp>
#include <Colors.hpp>

#include <ctime>
#include <sstream>

std::ofstream Logger::_logFile(LOGFILE);
std::stringstream Logger::inputStream;

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
	if (!ENABLE_LOGGING)
		return;

	if (!_logFile.is_open())
	{
		printWarning("There's no logfile, no logs will be saved.");
		return;
	}

	std::string timestamp = getTimeStamp();

	_logFile.write(timestamp.c_str(), timestamp.size());
	_logFile.write(msg.c_str(), msg.size());
	_logFile.write("\n", 1);
	_logFile.flush();
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
	if (!ENABLE_DEBUGGING)
		return;
	std::string str = inputStream.str();
	inputStream.str("");
	printDebug(str);

	if (ENABLE_DEBUG_LOGGING)
		log("[DEBUG] " + str);
}
