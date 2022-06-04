#pragma once

#include <sys/time.h>

namespace Webserver
{
	class ITimeoutable
	{
	public:
		virtual ~ITimeoutable() {}
		virtual void onTimeout() = 0;
		virtual timeval getLastCommunicated() const = 0;
	
	protected:
		timeval _lastCommunicated;
	};
}
