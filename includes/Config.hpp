#pragma once

#include <map>
#include <vector>

#include <Utility.hpp>
#include <Method.hpp>
#include <Logger.hpp>
#include <MimeTypes.hpp>
#include <AConfig.hpp>

struct Config : public AConfig
{
	Config();
	Config(const Config& rhs);
	~Config();
	Config& operator=(const Config& rhs);

	MimeTypes	mimeTypes;

	int			listenBacklog;
	int			bufferSize;
	std::string	mimeFilePath;
	bool		debugEnabled;
	bool		loggingEnabled;

private:
	map_type fillVariablesMap();

};
