#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>


namespace Webserver
{
	#define SPACE ' '

	class HtmlElement
	{
		public:
			friend class HtmlBuilder;

			bool 		isSelfClosing() const;
			std::string to_string(int indentAmonut) const;

		private:
			HtmlElement();
			HtmlElement(const std::string &tag, const std::string &tagBody, const std::string &content);

			std::string					_tag;
			std::string					_tagBody;
			std::string					_content;
			std::vector<HtmlElement>	_collection;
			const static int			_indentSize = 4;
	};
}
