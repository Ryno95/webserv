#pragma once

#include <iostream>
#include <map>
#include <algorithm>
#include <exception>
#include <string>

#include <Logger.hpp>
#include <Exception.hpp>
#include <Utility.hpp>
#include <config/ParseTreeUtility.hpp>

namespace Webserver
{
	class ATopDownParser
	{
	protected:
		ATopDownParser(StreamData* streamData) : _streamData(streamData), _finished(false)
		{
		}

		~ATopDownParser()
		{
			deleteKeywords();
		}

	public:
		void useBrackets()
		{
			deleteKeywords();
			_keywords["{"] = new BeginCommand(this);
		}

		void begin()
		{
			deleteKeywords();
			_keywords = createKeywords();
			_keywords["}"] = new EndCommand(this);
		}

		void end()
		{
			_finished = true;
		}

		bool readStream()
		{
			std::string line, key, value;
			while (_finished == false)
			{
				if (_streamData->stream.eof())
					return false;

				std::getline(_streamData->stream, line);

				line = Webserver::removeLeadingWhitespace(line);
				line = Webserver::removeTrailingWhitespace(line);

				if (line.size() == 0 || line.front() == '#')
					continue;

				size_t pos = std::min(line.find('\t'), line.find(' '));
				key = line.substr(0, pos);
				if (pos != std::string::npos)
					value = Webserver::removeLeadingWhitespace(line.substr(pos + 1, line.size() - pos));
				else
					value = "";

				std::map<std::string, ICommand*>::iterator it = _keywords.find(key);
				if (it == _keywords.end())
					throw std::runtime_error("Keyword '" + key + "' is not supported");
				else
					it->second->callback(value);

				_streamData->currentLine++;
			}
			return true;
		}

	void addKeyword(const std::string& keyword, ICommand* callback)
	{
		_keywords.insert(std::pair<std::string, ICommand*>(keyword, callback));
	}

	void setKeywords(const std::map<std::string, ICommand*>& newKeywords)
	{
		deleteKeywords();
		_keywords = newKeywords;
	}

	protected:

		virtual std::map<std::string, ICommand*> createKeywords() = 0;
		virtual void validate() = 0;

		StreamData* _streamData;
		std::map<std::string, ICommand*> _keywords;

	private:
		void deleteKeywords()
		{
			std::map<std::string, ICommand*>::iterator it = _keywords.begin();
			std::map<std::string, ICommand*>::iterator end = _keywords.end();

			while (it != end)
			{
				delete it->second;
				it++;
			}
			_keywords.clear();
		}

		bool _finished;
	};
}
