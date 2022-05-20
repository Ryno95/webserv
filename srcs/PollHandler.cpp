#include <PollHandler.hpp>
#include <Logger.hpp>
#include <Exception.hpp>
#include <Utility.hpp>

namespace Webserver
{
	std::vector<pollfd> PollHandler::_fds;

	void PollHandler::addPollfd(int fd)
	{
		pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN;
		_fds.push_back(pfd);
	}

	pollfd* PollHandler::findPollfd(int fd)
	{
		std::vector<pollfd>::iterator start = _fds.begin();
		std::vector<pollfd>::iterator end = _fds.end();

		while (start != end)
		{
			if (start->fd == fd)
				return &(*start);
			start++;
		}
		ERROR("fd '" << fd << "' is not added to the poll handler. This could be a serious bug.");
		throw ValueDoesNotExistException(toString(fd));
	}

	void PollHandler::setWriteFlag(int fd, bool enabled)
	{
		pollfd* pfd = findPollfd(fd);
		if (enabled)
			pfd->events = POLLIN | POLLOUT;
		else
			pfd->events = POLLIN;
	}

	bool PollHandler::canReadOrWrite(int fd)
	{
		pollfd* pfd = findPollfd(fd);
		return pfd->revents != 0;
	}

	bool PollHandler::canRead(int fd)
	{
		pollfd* pfd = findPollfd(fd);
		return BIT_ISSET(pfd->revents, POLLIN_BIT);
	}

	bool PollHandler::canWrite(int fd)
	{
		pollfd* pfd = findPollfd(fd);
		return BIT_ISSET(pfd->revents, POLLOUT_BIT);
	}

	/*
		If parameter fd is currently in the poll array, fd is removed.
		Otherwise nothing happens.
	*/
	void PollHandler::removePollfd(int fd)
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

	bool PollHandler::checkPoll()
	{
		int pollRet;

		pollRet = poll(&_fds.front(), _fds.size(), 1000); // 1000 ms is temporary
		if (pollRet == SYSTEM_ERR)
			throw SystemCallFailedException("Poll");
		else if (pollRet == 0)
			return false;
		return true;
	}
}
