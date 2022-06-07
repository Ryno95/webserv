#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <exception>

#include <Exception.hpp>
#include <Method.hpp>
#include <Utility.hpp>
#include <VariableParser.hpp>
#include <string>

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
					throw 
				*_dest = _parser.parse<T>(args);
			}

		private:
			T* _dest;
			VariableParser _parser;
		};



	protected:
		struct StreamData
		{
			StreamData(std::ifstream* fstream, uint line) : stream(fstream), currentLine(line) {}

			std::ifstream* stream;
			uint currentLine;
		};

	/*
		AConfigParser, abstract config parser class
	*/

	public:
		AConfigParser(StreamData streamData, std::map<std::string, ICommand*> keywords) : _keywords(keywords), _streamData(streamData)
		{
			readStream();
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
		void readStream()
		{
			std::string line, key, value;
			while (std::getline((*(std::istream*)_streamData.stream), line))
			{
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

				_streamData.currentLine++;
			}
		}

	protected:
		template<class T>
		void addChild(const std::string& args)
		{
			if (args.size() != 0)
				throw std::runtime_error("Unexpected tokens after keyword: " + args);
			_children.push_back(new T(_streamData));
		}

		virtual std::map<std::string, ICommand*> initKeywords() = 0;
		std::vector<AConfigParser*> _children;
		std::map<std::string, ICommand*> _keywords;
		StreamData _streamData;
	};
}
