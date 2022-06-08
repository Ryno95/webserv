#pragma once

#include <string>
#include <istream>

#include <VariableParser.hpp>

namespace Webserver
{
	class IBranch
	{
	protected:
		virtual ~IBranch() {}
	public:
		virtual void addChild(const std::string& args) = 0;
	};



	struct StreamData
	{
		StreamData(std::istream& istream, uint line) : stream(istream), currentLine(line) {}

		std::istream& stream;
		uint currentLine;
	};



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
		CreateChildCommand(IBranch* instance) : _instance(instance) {}
		~CreateChildCommand() {}

		void callback(const std::string& args)
		{
			_instance->addChild(args);
		}

	private:
		IBranch* _instance;
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


	class AConfigParser; // Forward declaration


	class BeginCommand : public ICommand
	{
	public:
		BeginCommand(AConfigParser* instance);
		~BeginCommand();

		void callback(const std::string& args);

	private:
		AConfigParser* _instance;
	};




	class EndCommand : public ICommand
	{
	public:
		EndCommand(AConfigParser* instance);
		~EndCommand();

		virtual void callback(const std::string& args);

	private:
		AConfigParser* _instance;
	};
}
