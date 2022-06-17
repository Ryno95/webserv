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

			std::vector<std::string> getDirEntries();

			std::string getHtmlPage();
		
		private:
			const std::string&	_root;
			HtmlBuilder			_builder;
	};

}
