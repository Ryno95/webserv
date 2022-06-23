
#include <autoIndex/AutoIndex.hpp>
#include <autoIndex/HtmlBuilder.hpp>
#include <Logger.hpp>

#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>

namespace Webserver
{
	AutoIndex::AutoIndex(const std::string &root) : _root(root), _builder(HtmlBuilder("html"))
	{
		const std::string 			htmlHeader("<!DOCTYPE html>\n");
		std::vector<std::string> 	dirEntries = getDirEntries();
		
		std::vector<std::string>::const_iterator it = dirEntries.begin();
		for(; it != dirEntries.end(); it++)
			_builder.addElement("body", "", HtmlBuilder("p")
					.addElement("a" , "href=" + *it, *it)
					.addElement("br", "", "")
					.build());
	}

	std::vector<std::string> AutoIndex::getDirEntries()
	{
		const std::string			currDir(".");
		DIR* 						dir;
		dirent* 					dirEntry;
		std::vector<std::string> 	entries;

		dir = opendir(_root.c_str());
		while (true)
		{
			dirEntry = readdir(dir);
			if (dirEntry == nullptr)
				break;
			if (currDir.compare(dirEntry->d_name) != 0)
			{
				if (dirEntry->d_type == DT_DIR)
					entries.push_back(std::string(dirEntry->d_name).append("/"));
				else
					entries.push_back(dirEntry->d_name);
			}
		}
		closedir(dir);
		return entries;
	}

	std::string AutoIndex::getHtmlPage()
	{
		return (_builder.build());
	}
}
