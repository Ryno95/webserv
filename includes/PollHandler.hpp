#pragma once

#include <vector>
#include <poll.h>

#include <config/ServerConfig.hpp>
#include <defines.hpp>

namespace Webserver
{
	class PollHandler
	{
	public:
		static bool checkPoll();
		static void addPollfd(int fd);
		static void removePollfd(int fd);

		static bool canReadOrWrite(int fd);
		static bool canWrite(int fd);
		static bool canRead(int fd);
		static void setWriteFlag(int fd, bool enabled);

	private:
		static pollfd* findPollfd(int fd);

		static std::vector<pollfd> _fds;

	};
}
