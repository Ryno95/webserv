#pragma once

#include <ASubscribeable.hpp>

namespace Webserver
{
	class ITickable
	{
	public:
		virtual ~ITickable() {};
		virtual void tick() = 0;
	};
}
