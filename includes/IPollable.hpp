#pragma once

namespace Webserver
{
	class IPollable
	{
	public:
		virtual ~IPollable() {}
		virtual void readHandler() = 0;
		virtual void writeHandler() = 0;
		virtual int getFd() const = 0;
	};

	class IPollableTickable : public IPollable
	{
	public:
		virtual ~IPollableTickable() {}
		virtual void tick() = 0;
	};
}
