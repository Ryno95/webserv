#pragma once

#include <vector>

#include <config/ServerConfig.hpp>
#include <Logger.hpp>
#include <MimeTypes.hpp>

namespace Webserver
{
	class AppConfig
	{
	public:
		friend class ConfigParser;

		AppConfig(); 
		AppConfig(const AppConfig& ref);
		~AppConfig();
		AppConfig& operator=(const AppConfig& ref);



	private:
		std::vector<ServerConfig> _serverConfigs;
		MimeTypes	_mimeTypes;

		uint		_listenBacklog;
		uint		_bufferSize;
		std::string	_mimeFilePath;
		bool		_debugEnabled;
		bool		_loggingEnabled;
		std::string	_logFile;

	};
}
 