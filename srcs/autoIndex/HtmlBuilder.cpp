#include <autoIndex/HtmlBuilder.hpp>
#include <iostream>

namespace Webserver
{

	HtmlBuilder::HtmlBuilder(std::string rootTag)
	{
		_root._tag = rootTag;
	}

	HtmlBuilder& HtmlBuilder::addElement(std::string elementTag, std::string tagBody, std::string elementContent)
		// : _tag(elementTag), _tagBody(tagBody), _content(elementContent)
	{
		_root._collection.push_back(HtmlElement(elementTag, tagBody, elementContent));
		return *this;
	}

	std::string HtmlBuilder::build()
	{
		return _root.to_string(0);
	}

}
