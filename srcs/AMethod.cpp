#include <AMethod.hpp>

#include <fstream>

AMethod::AMethod(Request const& request) : _request(request)
{
}

void AMethod::setInputFile(std::string filePath)
{
	_response.setBodyStream(filePath);
}

void AMethod::writeFile()
{
}
