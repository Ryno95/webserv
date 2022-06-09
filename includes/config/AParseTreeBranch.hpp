#pragma once

#include <vector>

#include <config/AParseTreeLeaf.hpp>

namespace Webserver
{
	template<class T, class U>
	class AParseTreeBranch : public AParseTreeLeaf<T>, public IBranch
	{
	public:
		typedef T data_type;
		typedef U child_type;

		virtual void addChild(const std::string& args)
		{
			if (args.size() != 0)
				throw std::runtime_error("Unexpected tokens after keyword: " + args);

			_children.push_back(new child_type(this->_streamData));
			_children.back()->useBrackets();
			if (_children.back()->readStream() == false)
				throw std::runtime_error("Unclosed section encountered.");
			_children.back()->validate();
			this->_data->_children.push_back(_children.back()->getData());
		}

	protected:
		AParseTreeBranch(StreamData* streamData) : AParseTreeLeaf<T>::AParseTreeLeaf(streamData)
		{
		}

		virtual ~AParseTreeBranch() {}

		std::vector<child_type*> _children;
	};
}
