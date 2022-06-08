#pragma once

#include <config/AConfigParser.hpp>

namespace Webserver
{
	template<class T>
	class AParseTreeLeaf : public AConfigParser
	{
	public:
		typedef T data_type;

		virtual data_type* getData() const = 0;

	protected:
		AParseTreeLeaf(StreamData* streamData) : AConfigParser::AConfigParser(streamData), _data(new data_type())
		{
		}

		AParseTreeLeaf(StreamData* streamData, std::map<std::string, ICommand*> keywords) : AConfigParser::AConfigParser(streamData, keywords), _data(new data_type())
		{
		}

		AParseTreeLeaf();
		virtual ~AParseTreeLeaf() {}

		data_type* _data;
	};
}
