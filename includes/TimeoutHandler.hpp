#pragma once

#include <vector>

#include <ASubscribeable.hpp>
#include <ITimeoutable.hpp>

namespace Webserver
{
	class TimeoutHandler : public ASubscribeable<ITimeoutable>
	{
	public:
		static TimeoutHandler& get(); // tmp singleton?
	private:
		static TimeoutHandler _singleton; // tmp singleton?


	public:
		TimeoutHandler();
		~TimeoutHandler();

		void update();
		timeval getTime() const;


	private:
		timeval _now;
	};
}
