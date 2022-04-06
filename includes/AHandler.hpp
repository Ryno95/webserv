#pragma once

#include <string>

class AHandler
{
public:

protected:
	AHandler();
	virtual ~AHandler() = 0;

	virtual bool handle() = 0;

	std::string _buffer;
};
