#pragma once

#include <vector>
#include <algorithm>

namespace Webserver
{
	typedef unsigned long ssize_t;

	template<class T>
	class ASubscribeable
	{
	protected:
		ASubscribeable() {}
		virtual ~ASubscribeable() {}

	public:
		virtual void update() = 0;


		virtual void add(T* instance)
		{
			_subscribers.push_back(instance);
		}

		virtual ssize_t remove(T* instance)
		{
			typename std::vector<T*>::const_iterator it = std::find(_subscribers.begin(), _subscribers.end(), instance);
			if (it == _subscribers.end())
				return -1;
			
			_subscribers.erase(it);
			return _subscribers.begin() - it;
		}


	protected:
		std::vector<T*> _subscribers;
	};
}
