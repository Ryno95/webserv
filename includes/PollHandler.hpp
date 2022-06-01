#pragma once

#include <vector>
#include <poll.h>

#include <config/ServerConfig.hpp>
#include <defines.hpp>
#include <IPollable.hpp>

namespace Webserver
{
	class PollHandler : public ASubscribeable<IPollable>
	{
	public:
		static PollHandler& get(); // tmp singleton?
	private:
		static PollHandler _singleton; // tmp singleton?

	public:
		PollHandler();
		~PollHandler();

		void update();
		void add(IPollable* instance);
		void remove(IPollable* instance);
		void setWriteEnabled(IPollable* instance, bool enabled);

	private:
		int getPollfdIndexOf(int fd);

		std::vector<pollfd> _fds;
	};
}
