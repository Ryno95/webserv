#pragma once

#include <config/ATopDownParser.hpp>

namespace Webserver
{
	template<class T>
	class AParseTreeLeaf : public ATopDownParser
	{
	public:
		typedef T data_type;

		data_type* getData() const
		{
			return _data;
		}

	protected:
		AParseTreeLeaf(StreamData* streamData) : ATopDownParser::ATopDownParser(streamData), _data(new data_type())
		{
		}

		virtual ~AParseTreeLeaf() {}

		data_type* _data;
	};
}
