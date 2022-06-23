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


			std::string getHtmlPage();
		
		private:
			void		getDirEntries();

			const std::string&			_root;
			HtmlBuilder					_builder;
			std::vector<std::string> 	_dirEntries;
	};

}
