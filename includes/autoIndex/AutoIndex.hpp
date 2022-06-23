#pragma once

#include <iostream>
#include <autoIndex/HtmlBuilder.hpp>
namespace Webserver
{
	class AutoIndex
	{
		public:
			
			AutoIndex(const std::string &root);
			~AutoIndex(){};

			const std::string	getHtmlPage();
		
		private:
			void		createDirEntries();

			const std::string			_root;
			HtmlBuilder					_builder;
			std::vector<std::string> 	_dirEntries;

			AutoIndex(const AutoIndex &ins);
			AutoIndex& operator=(const AutoIndex& rhs);
	};
}
