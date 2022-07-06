#pragma once

#include <vector>
#include <Logger.hpp>

namespace Webserver
{
	template<class T>
	class AComposite
	{
	public:
		void addChild(const T& toAdd)
		{
			_children.push_back(toAdd);
		}

		const std::vector<T>& getChildren() const
		{
			return _children;
		}

	protected:
		virtual ~AComposite() {}
	private:
		std::vector<T> _children;
	};
}
