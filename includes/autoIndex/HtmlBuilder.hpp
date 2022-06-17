#pragma once

#include <sstream>
#include <vector>
#include <dirent.h>

#include <autoIndex/HtmlElement.hpp>

namespace Webserver
{
	class HtmlBuilder
	{
		public:
			HtmlBuilder(std::string rootTag);

			HtmlBuilder&			addElement(std::string elementTag, std::string tagBody, std::string elementContent);
			std::string				build();

		private:
			HtmlElement _root;
	};
}
