#pragma once

#include <map>
#include <vector>

#include <Utility.hpp>
#include <Method.hpp>
#include <Logger.hpp>
#include <MimeTypes.hpp>
#include <AConfig.hpp>

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

private:
	map_type fillVariablesMap();

};
