#pragma once

#include <vector>

#include <observerApi/ITickable.hpp>

namespace Webserver
{
	class TickHandler : public ASubscribeable<ITickable>
	{
	public:
		static TickHandler& get(); // tmp singleton?
	private:
		static TickHandler _singleton; // tmp singleton?

	public:
		TickHandler();
		~TickHandler();

		void update();

	};
}
