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
		static void add(IPollable* instance);
		static void add(IPollableTickable* instance);
		static void remove(IPollable* instance);
		static void remove(IPollableTickable* instance);
		static void setWriteEnabled(IPollable* instance, bool enabled);

	private:
		static int getPollfdIndexOf(int fd);

		static std::vector<pollfd> _fds;
		static std::vector<IPollable*> _handlers;
		static std::vector<IPollableTickable*> _tickables;
	};
}
