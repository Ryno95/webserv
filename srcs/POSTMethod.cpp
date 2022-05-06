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
	const std::string	root("root/uploads/"); // Will be replaced by config file
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
	
	std::cout << "Filename: " << fileName << " Target: " << _request.getTarget() << std::endl;
	outfile->open(fileName, std::ios_base::app);
	if (!outfile->is_open())
	{
		perror("File creation error: ");
		_response->setStatusCode(HttpStatusCodes::NOT_FOUND);
		delete outfile;
		return _response;
	}
	// Writing process will be in chunks
	std::cout << "Body: "<< _request.getBody() << std::endl;
	*outfile << _request.getBody() << std::endl;
	delete outfile;
	setPostResponseHeaders();
	_response->setStatusCode(HttpStatusCodes::CREATED);

	return _response;
}

