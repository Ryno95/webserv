#pragma once

#include <string>
#include <map>

#include <defines.hpp>

class Request
{
public:
	Request(std::string query);
	~Request();

private:
	void parse();
	void parseBody();
	void parseRequestLine();
	void parseHeaderFields();


	std::string	_query;

	method		_method;
	std::string	_target;
	std::string	_version;
	std::map<std::string, std::string>	_headerFields;
	std::string	_body;

};
