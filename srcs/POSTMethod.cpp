#include <fstream>
#include <iostream>
#include <ctime>

#include <HttpStatusCode.hpp>
#include <POSTMethod.hpp>
#include <Logger.hpp>


POSTMethod::POSTMethod(const Request &request) : AMethod(request) {}

POSTMethod::~POSTMethod() {}

std::string POSTMethod::createFileName(const std::string &target)
{
	const std::string	timestamp_s = std::to_string(time(nullptr));
	const std::string	root("root/uploads/");
	std::string			fileName;
	fileName =  root;

	if (target == "/")
		fileName += timestamp_s;
	else
	{
		size_t found = target.find_last_of("/");
		fileName += target.substr(1, found);
		_location =  fileName;
		fileName += timestamp_s;
		fileName += target.substr(found + 1);
	}
	return (fileName);
}

void POSTMethod::setPostResponseHeaders()
{
	_response->addHeaderField("Location", _location);
	_response->addHeaderField("Content-Length", std::to_string(_request.getBodySize()));
}

Response* POSTMethod::process()
{
	const std::string	fileName  = createFileName(_request.getTarget());
	std::ofstream       *outfile = new std::ofstream();
	
	outfile->open(fileName, std::ios_base::app);
	std::cout << fileName << std::endl;
	if (!outfile->is_open())
	{
		std::cout << "[DEBUG] POSTMethod file creation failed\n";
		_response->setStatusCode(HttpStatusCodes::BAD_REQUEST);
		delete outfile;
		return _response;
	}

	*outfile << _request.getBody() << std::endl;
	outfile->close();
	setPostResponseHeaders();
	_response->setStatusCode(HttpStatusCodes::CREATED);

	return _response;
}

