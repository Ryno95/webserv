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
#include <config/GlobalConfig.hpp>
#include <Exception.hpp>

namespace Webserver
{
	Webserv::Webserv(const ServerConfig& config)
		: _config(config)
	{
		setup();
		PollHandler::add(_listenerFd, this);
		DEBUG("Created server instance on port: " << _config.port);
	}

	Webserv::~Webserv()
	{
		close(_listenerFd);
		PollHandler::remove(_listenerFd, this);

		DEBUG("Destroyed server instance on port: " << _config.port);
	}

	void Webserv::setup()
	{
		const int socketSwitch = 1;
		struct sockaddr_in servAddr;

		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(_config.port);
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

		if (listen(_listenerFd, GlobalConfig::get().listenBacklog) == SYSTEM_ERR) // store GlobalConfig as a member of this class?
			throw SystemCallFailedException("listen");
	}

	void Webserv::handleListener()
	{
		int fd = accept(_listenerFd, NULL, NULL);
		if (fd == SYSTEM_ERR)
		{
			WARN("Accept in our listener was blocking, so we continue");
		}
		else
			_clients.push_back(new Client(_config, fd));
	}

	void Webserv::removeClients()
	{
		DEBUG("Clients connected:" << _clients.size());
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

	void Webserv::checkTimeout() const
	{
		timeval now;
		gettimeofday(&now, NULL);
		for (size_t i = 0; i < _clients.size(); i++)
		{
			_clients[i]->checkTimeout(now);
		}
	}

	const ServerConfig& Webserv::getConfig() const
	{
		return _config;
	}

	void Webserv::readHandler()
	{
		handleListener();
	}

	void Webserv::writeHandler()
	{
		// not used for listener
	}

	void Webserv::tick()
	{
		checkTimeout();
		removeClients();
	}
}
