#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace Webserver
{
	class SendStream
	{
	public:
		SendStream(std::stringstream* stream);
		SendStream(std::fstream* stream);
		~SendStream();

		bool getIsFilled() const;
		void setIsFilled();
		uint getSize() const;

		size_t read(char* buffer, uint n);
		std::istream& operator<<(const std::string& str);

	private:
		SendStream(const SendStream& ref);
		SendStream& operator=(const SendStream& ref);

		std::iostream* _stream;
		bool _isFilled;
	};
}
