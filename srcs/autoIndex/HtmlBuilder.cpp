#include <autoIndex/HtmlBuilder.hpp>
#include <iostream>

namespace Webserver
{

	HtmlBuilder::HtmlBuilder(std::string rootTag)
	{
		_root._tag = rootTag;
	}

	HtmlBuilder& HtmlBuilder::addElement(std::string childTag, std::string tagBody, std::string childContent)
	{
		_root._collection.push_back(HtmlElement(childTag, tagBody, childContent));
		return *this;
	}

	std::string HtmlBuilder::build()
	{
		return _root.to_string(0);
	}

}
