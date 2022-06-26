#include <SendStream.hpp>
#include <Logger.hpp>

namespace Webserver
{
	SendStream::SendStream(std::stringstream* stream) : _stream(stream), _isFilled(false)
	{
	}

	SendStream::SendStream(std::fstream* stream) : _stream(stream), _isFilled(true)
	{
	}

	SendStream::~SendStream()
	{
		delete _stream;
	}

	SendStream::SendStream(const SendStream& ref)
	{
		*this = ref;
	}

	SendStream& SendStream::operator=(const SendStream& ref)
	{
		_stream = ref._stream;
		_isFilled = ref._isFilled;
		return *this;
	}

	bool SendStream::getIsFilled() const
	{
		return _isFilled;
	}

	void SendStream::setIsFilled()
	{
		_isFilled = true;
	}

	uint SendStream::getSize() const
	{
		_stream->seekg(0, std::ios_base::end);
		uint n = _stream->tellg();
		_stream->seekg(0);
		return n;
	}

	size_t SendStream::read(char* buffer, uint n)
	{
		_stream->read(buffer, n);
		return _stream->gcount();
	}

	std::istream& SendStream::operator<<(const std::string& str)
	{
		_stream->clear();
		*_stream << str;
		return *_stream;
	}
}
