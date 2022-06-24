#include <methods/TargetInfo.hpp>
#include <sys/stat.h>

namespace Webserver
{
	TargetInfo::TargetInfo(std::string &target) : _target(target)
	{
		_entryExists = stat(target.c_str(), &_stats) == 0;
	}
	
	bool TargetInfo::isDir()
	{
		
	}
}
