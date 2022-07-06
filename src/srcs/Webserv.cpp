#include <stdexcept>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <defines.hpp>
#include <Logger.hpp>
#include <Webserv.hpp>
#include <Exception.hpp>
#include <TickHandler.hpp>

namespace Webserver
{

#pragma region Singleton AppConfig

	AppConfig* Webserv::_appConfig;
	void Webserv::config(AppConfig* config)
	{
		_appConfig = config;
	}

	const AppConfig& Webserv::config()
	{
		return *_appConfig;
	}

#pragma endregion

	Webserv::Webserv(const ServerConfig& config)
		: _config(config)
	{
		setup();
		PollHandler::get().add(this);
		TickHandler::get().add(this);

		DEBUG("Created server instance on port: " << _config.getPort());
	}

	Webserv::~Webserv()
	{
		close(_listenerFd);
		PollHandler::get().remove(this);
		TickHandler::get().remove(this);

		DEBUG("Destroyed server instance on port: " << _config.getPort());
	}

	void Webserv::setup()
	{
		const int socketSwitch = 1;
		struct sockaddr_in servAddr;

		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(_config.getPort());
		servAddr.sin_addr.s_addr = INADDR_ANY;

		_listenerFd = socket(AF_INET, SOCK_STREAM, STD_TCP);
		if (_listenerFd == SYSTEM_ERR)
			throw SystemCallFailedException("Socket");

		if (setsockopt(_listenerFd, SOL_SOCKET, SO_REUSEADDR,
					(char *)&socketSwitch, sizeof(socketSwitch)) == SYSTEM_ERR)
			throw SystemCallFailedException("setsockopt");

		if (bind(_listenerFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SYSTEM_ERR)
			throw SystemCallFailedException("Bind");

		if (fcntl(_listenerFd, F_SETFL, O_NONBLOCK) == SYSTEM_ERR)
			throw SystemCallFailedException("fcntl");

		if (listen(_listenerFd, config().getListenBacklog()) == SYSTEM_ERR)
			throw SystemCallFailedException("listen");
	}

	int Webserv::getFd() const
	{
		return _listenerFd;
	}

	void Webserv::checkClientsStatus()
	{
		size_t size = _clients.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_clients[i]->needsRemove())
			{
				delete _clients[i];
				_clients.erase(_clients.begin() + i);
				i--;
				size--;
			}
		}
	}

	const ServerConfig& Webserv::getConfig() const
	{
		return _config;
	}

	void Webserv::onRead()
	{
		int fd = accept(_listenerFd, NULL, NULL);
		if (fd == SYSTEM_ERR)
		{
			WARN("Accept in our listener was blocking, so we continue");
		}
		else
			_clients.push_back(new Client(_config, fd));
	}

	void Webserv::onWrite()
	{
		// not used for listener
	}

	void Webserv::onTick()
	{
		checkClientsStatus();
	}
}
