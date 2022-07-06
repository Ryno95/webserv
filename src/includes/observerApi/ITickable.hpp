#pragma once

#include <observerApi/ASubscribeable.hpp>

namespace Webserver
{
	class ITickable
	{
	public:
		virtual ~ITickable() {};
		virtual void onTick() = 0;
	};
}
