#pragma once

#include <poll.h>

#include <iostream>
#include <string>
#include <vector>

#include <Client.hpp>
#include <config/AppConfig.hpp>
#include <config/ServerConfig.hpp>
#include <IPollable.hpp>
#include <ITickable.hpp>
#include <PollHandler.hpp>

namespace Webserver
{

	class Client; // forward declaration

	class Webserv : public IPollable, public ITickable
	{
			static AppConfig* _appConfig;

		public:
			static void config(AppConfig* config);
			static const AppConfig& config();

			Webserv(const ServerConfig& config);
			~Webserv();

			int getFd() const;
			void onRead();
			void onWrite();
			void onTick();
			const ServerConfig& getConfig() const;
			void checkClientsStatus();

		private:
			void setup();

			const ServerConfig& _config;

			int						_listenerFd;
			std::vector<Client*>	_clients;
	};
}
