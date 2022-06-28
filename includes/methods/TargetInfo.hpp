#pragma once

#include <iostream>
#include <sys/stat.h>

namespace Webserver
{
	class TargetInfo
	{
		public:
			TargetInfo(const std::string& target);
			
			bool isDir() const;
			bool isWriteable() const;
			bool isExecutable() const;
			bool entryExists() const;

			const std::string& getTarget() const;

		private:
			std::string		_target;
			struct stat		_stats;
			bool			_entryExists;
	};
	
}
