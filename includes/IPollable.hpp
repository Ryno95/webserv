#pragma once

#include <ASubscribeable.hpp>

namespace Webserver
{
	class IPollable
	{
	public:
		virtual ~IPollable() {}
		virtual void onRead() = 0;
		virtual void onWrite() = 0;
		virtual int getFd() const = 0;
	};
}
