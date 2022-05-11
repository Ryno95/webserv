#pragma once

#include <map>
#include <vector>

#include <Utility.hpp>
#include <Method.hpp>
#include <Logger.hpp>
#include <MimeTypes.hpp>
#include <config/AConfig.hpp>

namespace Webserver
{
	struct GlobalConfig : public AConfig
	{
		GlobalConfig(); 
		GlobalConfig(const GlobalConfig& rhs);
		~GlobalConfig();
		GlobalConfig& operator=(const GlobalConfig& rhs);

		MimeTypes	mimeTypes;

		int			listenBacklog;
		int			bufferSize;
		std::string	mimeFilePath;
		bool		debugEnabled;
		bool		loggingEnabled;
		std::string	logFile;

		static const GlobalConfig& get();
		static void set(const GlobalConfig& config);
		void validate() const;

	private:
		map_type fillVariablesMap();

		static GlobalConfig _current;

	};
	
}
