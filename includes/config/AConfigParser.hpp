#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <exception>
#include <string>

#include <Logger.hpp>
#include <Exception.hpp>
#include <Method.hpp>
#include <Utility.hpp>
#include <VariableParser.hpp>

namespace Webserver
{
	class AConfigParser
	{

	/*
		ICommand and concrete commands
	*/

	protected:
		class ICommand
		{
		public:
			virtual void callback(const std::string& args) = 0;
			virtual ~ICommand() {}
		};



		template<class T>
		class CreateChildCommand : public ICommand
		{
		public:
			CreateChildCommand(AConfigParser* instance) : _instance(instance) {}
			~CreateChildCommand() {}

			void callback(const std::string& args)
			{
				_instance->addChild<T>(args);
			}

		private:
			AConfigParser* _instance;
		};



		template<class T>
		class ParseVariableCommand : public ICommand
		{
		public:
			ParseVariableCommand(T* dest) : _dest(dest) {}
			~ParseVariableCommand() {}

			void callback(const std::string& args)
			{
				if (args.size() == 0)
					throw std::runtime_error("No value assigned to variable");
				*_dest = _parser.parse<T>(args);
			}

		private:
			T* _dest;
			VariableParser _parser;
		};

		class BeginCommand : public ICommand
		{
		public:
			BeginCommand(AConfigParser* instance) : _instance(instance) {}
			~BeginCommand() {}

			void callback(const std::string& args)
			{
				(void)args;
				_instance->begin();
			}

		private:
			AConfigParser* _instance;
		};

		class EndCommand : public ICommand
		{
		public:
			EndCommand(AConfigParser* instance) : _instance(instance) {}
			~EndCommand() {}

			virtual void callback(const std::string& args)
			{
				(void)args;
				_instance->end();
			}

		private:
			AConfigParser* _instance;
		};



	protected:
		struct StreamData
		{
			StreamData(std::istream& istream, uint line) : stream(istream), currentLine(line) {}

			std::istream& stream;
			uint currentLine;
		};



	/*
		AConfigParser, abstract config parser class
	*/

	protected:
		AConfigParser(StreamData* streamData) : _streamData(streamData), _finished(false)
		{
			_keywords["{"] = new BeginCommand(this);
		}

		AConfigParser(StreamData* streamData, std::map<std::string, ICommand*> keywords) : _streamData(streamData), _keywords(keywords), _finished(false)
		{
		}

		~AConfigParser()
		{
			std::map<std::string, ICommand*>::iterator it = _keywords.begin();
			std::map<std::string, ICommand*>::iterator end = _keywords.end();

			while (it != end)
			{
				delete it->second;
				it++;
			}
		}

	private:
		void begin()
		{
			_keywords = createKeywords();
			_keywords["}"] = new EndCommand(this);
		}

		void end()
		{
			_finished = true;
		}

	protected:
		bool finishedCorrectly()
		{
			return _finished;
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

		template<class T>
		void addChild(const std::string& args);

		virtual std::map<std::string, ICommand*> createKeywords() = 0;
		virtual void validate() = 0;
		std::vector<AConfigParser*> _children;
		StreamData* _streamData;
		std::map<std::string, ICommand*> _keywords;

	private:
		bool _finished;
	};

	template<class T>
	void AConfigParser::addChild(const std::string& args)
	{
		if (args.size() != 0)
			throw std::runtime_error("Unexpected tokens after keyword: " + args);

		_children.push_back(new T(_streamData));
		if (_children.back()->readStream() == false)
			throw std::runtime_error("Unclosed section encountered.");
		_children.back()->validate();
	}
}
