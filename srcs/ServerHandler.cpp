#include <ServerHandler.hpp>
#include <Logger.hpp>

std::vector<Webserv*>	ServerHandler::_servers;
std::vector<pollfd>		ServerHandler::_fds;

void ServerHandler::addPollfd(int fd)
{
	pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	_fds.push_back(pfd);
}

pollfd* ServerHandler::findPollfd(int fd)
{
	std::vector<pollfd>::iterator start = _fds.begin();
	std::vector<pollfd>::iterator end = _fds.end();

	while (start != end)
	{
		if (start->fd == fd)
			return &(*start);
		start++;
	}
	throw std::runtime_error("WHUT THE FUUUUCK");
}

void ServerHandler::setPollOut(int fd, bool enabled)
{
	pollfd* pfd = findPollfd(fd);
	if (enabled)
		pfd->events = POLLIN | POLLOUT;
	else
		pfd->events = POLLIN;
}

bool ServerHandler::isPollSet(int fd)
{
	pollfd* pfd = findPollfd(fd);
	return pfd->revents != 0;
}

bool ServerHandler::isPollInSet(int fd)
{
	pollfd* pfd = findPollfd(fd);
	return BIT_ISSET(pfd->revents, POLLIN_BIT);
}

bool ServerHandler::isPollOutSet(int fd)
{
	pollfd* pfd = findPollfd(fd);
	return BIT_ISSET(pfd->revents, POLLOUT_BIT);
}

/*
	If parameter fd is currently in the poll array, fd is removed.
	Otherwise nothing happens.
*/
void ServerHandler::removePollfd(int fd)
{
	std::vector<pollfd>::iterator start = _fds.begin();
	std::vector<pollfd>::iterator end = _fds.end();

	while (start != end)
	{
		if (start->fd == fd)
		{
			_fds.erase(start);
			return;
		}
		start++;
	}
	_fds.erase(start);

	WARN("Tried to remove pollfd " << fd << ", but we were not polling for that.");
}

void ServerHandler::run()
{
	int pollRet;

	while (true)
	{
		pollRet = poll(&_fds.front(), _fds.size(), 1000); // 1000 ms is temporary
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
	_servers.push_back(new Webserv(config)); // no delete of this heap yet
}
