#include <algorithm>

#include <PollHandler.hpp>
#include <Logger.hpp>
#include <Exception.hpp>
#include <Utility.hpp>

namespace Webserver
{
	/*
		Singleton
	*/
	PollHandler PollHandler::_singleton;

	PollHandler& PollHandler::get()
	{
		return _singleton;
	}


	/*
		Object
	*/

	PollHandler::PollHandler()
	{
	}

	PollHandler::~PollHandler()
	{
	}

	void PollHandler::update()
	{
		int pollRet;

		pollRet = poll(&_fds.front(), _fds.size(), 1000); // 1000 ms is temporary
		if (pollRet == SYSTEM_ERR)
			throw SystemCallFailedException("Poll");

		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (BIT_ISSET(_fds[i].revents, POLLIN_BIT))
				_subscribers[i]->onRead();
			if (BIT_ISSET(_fds[i].revents, POLLOUT_BIT))
				_subscribers[i]->onWrite();
		}
	}

	void PollHandler::add(IPollable* instance)
	{
		pollfd pfd;
		pfd.fd = instance->getFd();
		pfd.events = POLLIN;
		_fds.push_back(pfd);
		ASubscribeable::add(instance);
	}

	void PollHandler::remove(IPollable* instance)
	{
		ASubscribeable::remove(instance);
		_fds.erase(_fds.begin() + getPollfdIndexOf(instance->getFd()));
	}

	void PollHandler::setWriteEnabled(IPollable* fd, bool enabled)
	{
		int index = getPollfdIndexOf(fd->getFd());
		if (index == -1)
			return;

		if (enabled)
			_fds[index].events = POLLIN | POLLOUT;
		else
			_fds[index].events = POLLIN;
	}

	int PollHandler::getPollfdIndexOf(int fd)
	{
		std::vector<pollfd>::iterator iter = _fds.begin();
		std::vector<pollfd>::iterator end = _fds.end();

		while (iter != end)
		{
			if (iter->fd == fd)
				return iter - _fds.begin();
			iter++;
		}

		WARN("Fd '" << fd << "' is not added to the poll handler. Have you correctly subscribed to PollHandler?");
		return -1;
	}
}
