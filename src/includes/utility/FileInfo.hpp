#pragma once

#include <iostream>
#include <sys/stat.h>

namespace Webserver
{
	class FileInfo
	{
		public:
			FileInfo(const std::string& fileName, const std::string& path);

			bool isDir() const;
			bool isWriteable() const;
			bool isExecutable() const;
			bool isReadable() const;
			bool entryExists() const;

			const std::string& getPath() const;
			const std::string& getFullPath() const;
			const std::string& getFileName() const;

		private:
			std::string		combinePath(const std::string& path, const std::string& fileName);

			std::string		_path;
			std::string		_fileName;
			std::string		_fullPath;
			struct stat		_stats;
			bool			_entryExists;
	};
	
}
