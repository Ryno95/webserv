#pragma once

#include <config/ParseTreeUtility.hpp>

namespace Webserver
{
	template<class T>
	class Parser
	{
	public:
		Parser(std::istream& stream) : _streamData(new StreamData(stream, 1))
		{
			_rootParser = new T(_streamData);
		}

		~Parser()
		{
			delete _streamData;
		};

		typename T::data_type* parse()
		{
			_rootParser->setKeywords(_rootParser->createKeywords());
			_rootParser->readStream();
			_rootParser->validate();
			return _rootParser->getData();
		}

	private:
		StreamData* _streamData;
		T* _rootParser;
	};
}
