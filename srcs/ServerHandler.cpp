#include <ServerHandler.hpp>
#include <Logger.hpp>

std::vector<Webserv*>	ServerHandler::_servers;
std::vector<pollfd>		ServerHandler::_fds;

pollfd& ServerHandler::addPollfd()
{
	_fds.push_back(pollfd());
	return _fds.back();
}

/*
	If parameter fd is currently in the poll array, fd is removed.
	Otherwise nothing happens.
*/
void ServerHandler::removePollfd(const pollfd& fd)
{
	std::vector<pollfd>::iterator start = _fds.begin();
	std::vector<pollfd>::iterator end = _fds.end();

	while (start != end)
	{
		if (&(*start) == &fd)
		{
			_fds.erase(start);
		}
		start++;
	}
}

void ServerHandler::run()
{
	int pollRet;

	while (true)
	{
		pollRet = poll(&_fds.front(), _fds.size(), 100); // 100 ms is temporary
		if (pollRet == SYSTEM_ERR)
			throw std::runtime_error("poll() failed");

		for (size_t i = 0; i < _servers.size(); i++)
		{
			_servers[i]->handle();
		}
	}
}

void ServerHandler::addServer(const ServerConfig& config)
{
	_servers.push_back(new Webserv(config));
}
