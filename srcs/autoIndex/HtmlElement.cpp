#include <autoIndex/HtmlElement.hpp>


namespace Webserver
{

	HtmlElement::HtmlElement() {}

	HtmlElement::HtmlElement(const std::string &tag, const std::string &tagBody, const std::string &content)
			: _tag(tag), _tagBody(tagBody), _content(content) {}

	HtmlElement::HtmlElement(const std::string &tag)
			: _tag(tag), _tagBody(""), _content("") {}
	
	bool HtmlElement::isSelfClosing() const
	{
		const char*	selfClosingTags[] = {"img", "br", "link", NULL};

		for (int i = 0; selfClosingTags[i]; i++)
		{
			if (_tag.find(selfClosingTags[i]) != std::string::npos)
				return true;
		}
		return false;
	}

	std::string HtmlElement::toString(int indentAmonut) const
	{
		std::stringstream oss;

		oss << std::string(_indentSize * indentAmonut, SPACE) << "<" << _tag;
		if (_tagBody.size())
			oss << " " << _tagBody << std::endl;
		oss << ">" << std::endl;

		if (_content.size())
			oss << std::string(_indentSize * (indentAmonut + 1), SPACE) << _content << std::endl;
		std::vector<HtmlElement>::const_iterator element = _collection.begin();

		for (; element != _collection.end(); element++)
			oss << element->toString(indentAmonut);

		if (!this->isSelfClosing())
			oss << std::string(_indentSize * indentAmonut, SPACE) << "</" << _tag << ">" << std::endl;

		return oss.str();
	}
}
