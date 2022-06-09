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
		friend class AppConfigParser;

		AppConfig(); 
		AppConfig(const AppConfig& ref);
		~AppConfig();
		AppConfig& operator=(const AppConfig& ref);

		const std::vector<ServerConfig> getServerConfigs() const;
		const MimeTypes& getMimeTypes() const;
		uint getListenBacklog() const;
		uint getBufferSize() const;
		bool isDebugEnabled() const;
		bool isDebugLoggingEnabled() const;
		const std::string& getLogFileName() const;

		std::vector<ServerConfig*> _children;

	private:
		MimeTypes	_mimeTypes;

		uint		_listenBacklog;
		uint		_bufferSize;
		std::string	_mimeFilePath;
		bool		_debugEnabled;
		bool		_loggingEnabled;
		std::string	_logFile;

	};
}
