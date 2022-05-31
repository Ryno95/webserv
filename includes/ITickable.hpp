#pragma once

#include <ASubscribeable.hpp>

namespace Webserver
{
	class ITickable
	{
	public:
		virtual ~ITickable() {};
		virtual void onTick() = 0;
	};
}
