#include <PollHandler.hpp>
#include <Logger.hpp>
#include <Exception.hpp>
#include <Utility.hpp>

namespace Webserver
{
	std::vector<pollfd> PollHandler::_fds;
	std::vector<IPollable*> PollHandler::_handlers;
	std::vector<IPollableTickable*> PollHandler::_tickables;

	void PollHandler::loop()
	{
		while (true)
		{
			int pollRet;

			pollRet = poll(&_fds.front(), _fds.size(), 1000); // 1000 ms is temporary
			if (pollRet == SYSTEM_ERR)
				throw SystemCallFailedException("Poll");

			for (size_t i = 0; i < _tickables.size(); i++)
			{
				_tickables[i]->tick();
			}

			for (size_t i = 0; i < _fds.size(); i++)
			{
				if (BIT_ISSET(_fds[i].revents, POLLIN_BIT))
					_handlers[i]->readHandler();
				if (BIT_ISSET(_fds[i].revents, POLLOUT_BIT))
					_handlers[i]->writeHandler();
			}
		}
	}

	void PollHandler::add(int fd, IPollableTickable* instance)
	{
		_tickables.push_back(instance);
		add(fd, (IPollable*)instance);
	}

	void PollHandler::add(int fd, IPollable* instance)
	{
		pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN;
		_fds.push_back(pfd);
		_handlers.push_back(instance);
	}

	void PollHandler::remove(int fd)
	{
		int index = getIndexOf(fd);
		if (index == -1)
			return;

		_fds.erase(_fds.begin() + index);
		_handlers.erase(_handlers.begin() + index);
	}

	void PollHandler::setWriteEnabled(int fd, bool enabled)
	{
		int index = getIndexOf(fd);
		if (index == -1)
			return;

		if (enabled)
			_fds[index].events = POLLIN | POLLOUT;
		else
			_fds[index].events = POLLIN;
	}

	int PollHandler::getIndexOf(int fd)
	{
		std::vector<pollfd>::iterator iter = _fds.begin();
		std::vector<pollfd>::iterator end = _fds.end();

		while (iter != end)
		{
			if (iter->fd == fd)
				return iter - _fds.begin();
			iter++;
		}

		WARN("Fd '" << fd << "' is not added to the poll handler. This could be a serious bug.");
		return -1;
	}
}
