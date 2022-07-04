#include <autoIndex/HtmlBuilder.hpp>
#include <iostream>

namespace Webserver
{
	HtmlBuilder::HtmlBuilder(std::string rootTag)
	{
		_root._tag = rootTag;
	}

	HtmlBuilder& HtmlBuilder::addElement(std::string elementTag, std::string tagBody, std::string elementContent)
	{
		_root._collection.push_back(HtmlElement(elementTag, tagBody, elementContent));
		return *this;
	}

	HtmlBuilder& HtmlBuilder::addElement(std::string elementTag)
	{
		_root._collection.push_back(HtmlElement(elementTag));
		return *this;
	}

	std::string HtmlBuilder::build()
	{
		const int rootIndentAmount = 0;

		return _root.toString(rootIndentAmount);
	}

}
