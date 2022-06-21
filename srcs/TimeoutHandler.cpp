#include <algorithm>
#include <iostream>

#include <defines.hpp>
#include <TimeoutHandler.hpp>

namespace Webserver
{
	/*
		Singleton
	*/
	TimeoutHandler TimeoutHandler::_singleton;

	TimeoutHandler& TimeoutHandler::get()
	{
		return _singleton;
	}


	/*
		Object
	*/

	TimeoutHandler::TimeoutHandler()
	{
	}

	TimeoutHandler::~TimeoutHandler()
	{
	}

	void TimeoutHandler::update()
	{
		gettimeofday(&_now, nullptr);
		for (size_t i = 0; i < _subscribers.size(); i++)
		{
			timeval lastCommunicated = _subscribers[i]->getLastCommunicated();
			if (((_now.tv_sec - lastCommunicated.tv_sec) * 1000) + ((_now.tv_usec - lastCommunicated.tv_usec) / 1000) >= TIMEOUT_MS)
				_subscribers[i]->onTimeout();
		}
	}

	timeval TimeoutHandler::getTime() const
	{
		return _now;
	}
}
