#pragma once

#include <string>

namespace Webserver
{
	class ICommand
	{
	public:
		virtual void callback(const std::string& args) = 0;
		virtual ~ICommand() {}
	};
}
