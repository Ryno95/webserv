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

			child_type* newChild = new child_type(this->_streamData);
			_children.push_back(newChild);

			newChild->useBrackets();
			if (newChild->readStream() == false)
				throw std::runtime_error("Unclosed settings block encountered.");
		}

		void fillData()
		{
			AParseTreeLeaf<T>::fillData();

			for (size_t i = 0; i < _children.size(); i++)
			{
				_children[i]->fillData();
				this->_data->addChild(_children[i]->getData());
			}
		}

	protected:
		AParseTreeBranch(StreamData* streamData) : AParseTreeLeaf<T>::AParseTreeLeaf(streamData)
		{
		}

		virtual ~AParseTreeBranch()
		{
			for (size_t i = 0; i < _children.size(); i++)
			{
				delete _children[i];
			}
			
		}

		std::vector<child_type*> _children;
	};
}
