#include <methods/TargetInfo.hpp>
#include <sys/stat.h>

namespace Webserver
{
	TargetInfo::TargetInfo(std::string target) : _target(target)
	{
		_entryExists = stat(target.c_str(), &_stats) == 0;
	}
	
	bool TargetInfo::isDir() const
	{
		return _entryExists && _stats.st_mode & S_IFDIR;
	}

	bool TargetInfo::isWriteable() const
	{
		return _entryExists && _stats.st_mode & S_IWUSR;
	}

	bool TargetInfo::isExecutable() const
	{
		return _entryExists && _stats.st_mode & S_IXUSR;
	}
	
	bool TargetInfo::entryExists() const
	{
		return _entryExists;
	}

	const std::string& TargetInfo::getTarget() const
	{
		return _target;
	}
}
