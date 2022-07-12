#include <FileInfo.hpp>
#include <sys/stat.h>
#include <Logger.hpp>

namespace Webserver
{
	FileInfo::FileInfo(const std::string& fileName, const std::string& path) : 
		_path(path),
		_fileName(fileName),
		_fullPath(combinePath(path, fileName))
	{
		_entryExists = stat(_fullPath.c_str(), &_stats) == 0;
	}

	bool FileInfo::isDir() const
	{
		return _entryExists && _stats.st_mode & S_IFDIR;
	}

	bool FileInfo::isWriteable() const
	{
		return _entryExists && _stats.st_mode & S_IWUSR;
	}

	bool FileInfo::isExecutable() const
	{
		return _entryExists && _stats.st_mode & S_IXUSR;
	}
	
	bool FileInfo::isReadable() const
	{
		return _entryExists && _stats.st_mode & S_IRUSR;
	}

	bool FileInfo::entryExists() const
	{
		return _entryExists;
	}

	const std::string& FileInfo::getPath() const
	{
		return _path;
	}

	const std::string& FileInfo::getFullPath() const
	{
		return _fullPath;
	}

	const std::string& FileInfo::getFileName() const
	{
		return _fileName;
	}

	// Root can't be empty
	// Atleast "/" or "//" as target
	// Accept ony the full path or no root as viable options
	std::string FileInfo::combinePath(const std::string &path, const std::string& fileName)
	{
		const size_t	targetSlashPos = fileName.find('/');
		const size_t	rootSlashPos = path.find_last_of('/');
		const size_t	rootSize = path.size();

		if (fileName.find(path) != std::string::npos)
		{
			if (targetSlashPos == 0)
				return fileName.substr(1, fileName.size());
			return fileName;
		}
		if (rootSlashPos == rootSize - 1 && targetSlashPos == 0)
		{
			return std::string(path + fileName.substr(targetSlashPos + 1, fileName.size()));
		}	
		else if ((rootSlashPos == rootSize - 1 && targetSlashPos != 0)
					|| (rootSlashPos != rootSize - 1 && targetSlashPos == 0))
		{
			return std::string(path + fileName);
		}
		else
		{
			return std::string(path + "/" + fileName);
		}
	}
}
