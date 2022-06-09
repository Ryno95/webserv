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
