#pragma once

#include <config/ATopDownParser.hpp>

namespace Webserver
{
	template<class T>
	class AParseTreeLeaf : public ATopDownParser
	{
	public:
		typedef T data_type;

		data_type* getData()
		{
			data_type* tmp = _data;
			_data = nullptr;
			return tmp;
		}

		virtual void fillData()
		{
			std::vector<std::pair<ICommand*, std::string> >::iterator it = _commands.begin();
			std::vector<std::pair<ICommand*, std::string> >::iterator end = _commands.end();

			while (it != end)
			{
				it->first->callback(it->second);
				it++;
			}
			_commands.clear();

			validate();
		}

	protected:
		AParseTreeLeaf(StreamData* streamData) : ATopDownParser::ATopDownParser(streamData), _data(new data_type())
		{
		}

		virtual ~AParseTreeLeaf()
		{
			if (_data != nullptr)
				delete _data;
		}

		data_type* _data;
	};
}
