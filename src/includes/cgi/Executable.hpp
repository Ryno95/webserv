#pragma once

#include <FileInfo.hpp>

#include <map>
#include <string>

namespace Webserver
{
	class Executable
	{
	public:
		Executable(const std::string& targetFile);
		Executable(const std::string& targetFile, const std::string& scriptInterpretor);
		~Executable();

		void execute();

		Executable& setEnv(const std::map<std::string, std::string>& env);

	private:
		std::string	findExecutablePath();

		char** getCStyleEnv() const;
		char** getCStyleArgv(const std::string& targetFilePath) const;

		std::map<std::string, std::string> _env;
		std::string _scriptInterpretor;
		std::string	_targetFile;
		std::string _executablePath;
	};
}
