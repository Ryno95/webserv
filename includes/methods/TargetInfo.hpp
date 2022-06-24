#pragma once

#include <iostream>

namespace Webserver
{
	class TargetInfo
	{
		public:
			TargetInfo(std::string &target);
			
			bool isDir();

		private:
			int				_stats;
			std::string&	_target;
			bool			_entryExists;
	};

	

	
}