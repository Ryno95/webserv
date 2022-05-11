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

		static bool isPollSet(int fd);
		static bool isPollOutSet(int fd);
		static bool isPollInSet(int fd);
		static void setPollOut(int fd, bool enabled);

	private:
		static pollfd* findPollfd(int fd);

		static std::vector<pollfd> _fds;

	};
}
