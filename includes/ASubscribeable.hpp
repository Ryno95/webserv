#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace Webserver
{
	/*
		Base subscribeable template class.
		Template argument should be the interface of the subscribers.
		Update should be called periodically to check all subscribers data against this object's state.
		Usually the subscriber's interface contains a function callback which can be called by this handler's update function.
	*/

	template<class T>
	class ASubscribeable
	{
		typedef T*	subscriber_type;
		typedef typename std::vector<subscriber_type>::iterator	iterator;

	public:
		virtual void update() = 0;

		/*
			Adds subscriber to the currently subscribed subscribers collection.
			Add does not check whether subscriber is already subscribed to the handler.
			Which means it's possible to subscribe multiple times with the same instance.
		*/
		virtual void add(subscriber_type subscriber)
		{
			_subscribers.push_back(subscriber);
		}

		/*
			If subscriber is currently subscribed, removes subscriber.
			Otherwise nothing happens.
		*/
		virtual void remove(subscriber_type subscriber)
		{
			iterator it = std::find(_subscribers.begin(), _subscribers.end(), subscriber);
			if (it == _subscribers.end())
				return;

			_subscribers.erase(it);
		}

	protected:
		ASubscribeable() {}
		virtual ~ASubscribeable() {}

		/*
			If subscriber is contained in subscribers, returns the index of subscriber.
			Otherwise returns -1.
		*/
		long indexOf(subscriber_type subscriber)
		{
			iterator it = std::find(_subscribers.begin(), _subscribers.end(), subscriber);
			if (it == _subscribers.end())
				return -1;

			return it - _subscribers.begin();
		}

		std::vector<subscriber_type> _subscribers;
	};
}
