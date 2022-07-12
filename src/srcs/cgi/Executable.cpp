#include <unistd.h>

#include <cgi/Executable.hpp>
#include <defines.hpp>
#include <Exception.hpp>
#include <FileInfo.hpp>

#include <Logger.hpp>

namespace Webserver
{
	Executable::Executable(const std::string& targetFile) :
		_targetFile(targetFile),
		_executablePath(targetFile)
	{
	}

	Executable::Executable(const std::string& targetFile, const std::string& scriptInterpretor) :
		_scriptInterpretor(scriptInterpretor),
		_targetFile(targetFile),
		_executablePath(findExecutablePath())
	{
	}

	Executable::~Executable()
	{
	}

	Executable& Executable::setEnv(const std::map<std::string, std::string>& env)
	{
		_env = env;
		return *this;
	}

	void Executable::execute()
	{
		char* const* argv = getCStyleArgv(_targetFile);
		char* const* env = getCStyleEnv();

		if (execve(_executablePath.c_str(), argv, env) == SYSTEM_ERR)
			throw SystemCallFailedException("execve()");
	}

	char** Executable::getCStyleArgv(const std::string& targetFilePath) const
	{
		const int argc = 2;
		char** argv;
		int it = 0;

		if (_scriptInterpretor.size() != 0)
		{
			argv = new char*[argc + 1];
			argv[0] = new char[_scriptInterpretor.size() + 1];
			std::strcpy(argv[0], _scriptInterpretor.c_str());
			it++;
		}
		else
			argv = new char*[argc];
		argv[it] = new char[targetFilePath.size() + 1];
		std::strcpy(argv[it], targetFilePath.c_str());
		argv[it + 1] = NULL;
		return argv;
	}

	char** Executable::getCStyleEnv() const
	{
		char **env = new char*[_env.size() + 1];
		std::map<std::string, std::string>::const_iterator it = _env.begin();
		std::map<std::string, std::string>::const_iterator end = _env.end();

		for (int i = 0; it != end; it++, i++)
		{
			std::string keyAndVal(it->first + "=" + it->second);
			env[i] = new char[keyAndVal.size() + 1];
			std::strcpy(env[i], keyAndVal.c_str());
		}
		env[_env.size()] = NULL;
		return env;
	}

	std::string	Executable::findExecutablePath()
	{
		std::string		allPaths(getenv("PATH"));
		size_t			SinglePathLen;
		int 			i = 0;

		SinglePathLen = allPaths.find(":");
		while (SinglePathLen != std::string::npos)
		{
			std::string singlePath(allPaths.substr(i, SinglePathLen - i));
			if (FileInfo(_scriptInterpretor, singlePath).isExecutable())
				return singlePath + "/" + _scriptInterpretor;
			i = SinglePathLen + 1;
			SinglePathLen = allPaths.find(":", i);
		}
		WARN("Interpretor executable not found with current PATH: " << _scriptInterpretor);
		return "";
	}
}
