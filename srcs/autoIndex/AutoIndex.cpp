
#include <autoIndex/AutoIndex.hpp>
#include <autoIndex/HtmlBuilder.hpp>
#include <Logger.hpp>
#include <Exception.hpp>

#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>

namespace Webserver
{
	AutoIndex::AutoIndex(const std::string &root) : _root(root), _builder(HtmlBuilder("html"))
	{
		const std::string 			htmlHeader("<!DOCTYPE html>\n");

		createDirEntries();
		std::vector<std::string>::const_iterator it = _dirEntries.begin();
		for(; it != _dirEntries.end(); it++)
			_builder.addElement("body", "", HtmlBuilder("p")
									.addElement("a" , "href=" + *it, *it)
									.addElement("br", "", "")
									.build()
								);
	}

	AutoIndex::AutoIndex(const AutoIndex &ins) : _root(ins._root), _builder(HtmlBuilder("html"))
	{
		*this = ins;
	}
	
	AutoIndex& AutoIndex::operator=(const AutoIndex& rhs)
	{
		(void)rhs;
		return (*this);
	}
	
	void AutoIndex::createDirEntries()
	{
		const std::string			currDirectory(".");
		DIR* 						dir;
		dirent* 					dirEntry;

		dir = opendir(_root.c_str());
		if (!dir)
			throw InvalidRequestException(HttpStatusCodes::FORBIDDEN);
		while (true)
		{
			dirEntry = readdir(dir);
			if (dirEntry == nullptr)
				break;
			if (currDirectory != std::string(dirEntry->d_name))
			{
				if (dirEntry->d_type == DT_DIR)
					_dirEntries.push_back(std::string(dirEntry->d_name).append("/"));
				else
					_dirEntries.push_back(dirEntry->d_name);
			}
		}
		closedir(dir);
	}

	const std::string AutoIndex::getHtmlPage()
	{
		return (_builder.build());
	}
}
