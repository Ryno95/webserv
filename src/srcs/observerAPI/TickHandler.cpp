#include <TickHandler.hpp>

#include <algorithm>
#include <iostream>

namespace Webserver
{
	/*
		Singleton
	*/
	TickHandler TickHandler::_singleton;

	TickHandler& TickHandler::get()
	{
		return _singleton;
	}


	/*
		Object
	*/

	TickHandler::TickHandler()
	{
	}

	TickHandler::~TickHandler()
	{
	}

	void TickHandler::update()
	{
		for (size_t i = 0; i < _subscribers.size(); i++)
		{
			_subscribers[i]->onTick();
		}
	}
}
