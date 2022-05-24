#pragma once

#include <vector>
#include <poll.h>

#include <config/ServerConfig.hpp>
#include <defines.hpp>
#include <IPollable.hpp>

namespace Webserver
{

	class PollHandler
	{
	public:
		static void loop();
		static void add(int fd, IPollable* instance);
		static void add(int fd, IPollableTickable* instance);
		static void remove(int fd);
		static void setWriteEnabled(int fd, bool enabled);

	private:
		static int getIndexOf(int fd);

		static std::vector<pollfd> _fds;
		static std::vector<IPollable*> _handlers;
		static std::vector<IPollableTickable*> _tickables;
	};
}
