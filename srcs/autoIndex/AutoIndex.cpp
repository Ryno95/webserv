
#include <autoIndex/AutoIndex.hpp>
#include <autoIndex/HtmlBuilder.hpp>

#include <dirent.h>
namespace Webserver
{
	AutoIndex::AutoIndex(const std::string &root) : _root(root), _builder(HtmlBuilder("html"))
	{
		const std::string htmlHeader("<!DOCTYPE html>\n");
		std::vector<std::string> dirEntries = getDirEntries();

		std::vector<std::string>::const_iterator it = dirEntries.begin();
		for(; it != dirEntries.end(); it++)
			_builder.addElement("body", "", HtmlBuilder("p").addElement("a" , "href=" + *it, *it).addElement("br", "", "").build());
	}

	std::vector<std::string> AutoIndex::getDirEntries()
	{
		dirent* dirEntry;
		DIR* dir;
		std::vector<std::string> entries;

		dir = opendir(_root.c_str());
		while (true)
		{
			dirEntry = readdir(dir);
			if (dirEntry == nullptr)
				break;
			entries.push_back(dirEntry->d_name);
		}
		closedir(dir);
		return entries;
	}

	std::string AutoIndex::getHtmlPage()
	{
		std::cout << _builder.build() << std::endl;
		return (_builder.build());
	}
	
}
