#include <fstream>
#include <iostream>

#include <HttpStatusCode.hpp>
#include <POSTMethod.hpp>


POSTMethod::POSTMethod(const Request &request) : AMethod(request) {}

POSTMethod::~POSTMethod() {}

Response* POSTMethod::process()
{
	std::string   		target = _request.getTarget();
	std::string			fileName;
	std::ofstream       *outfile = new std::ofstream();
	std::string         root("root/");
	std::string         index("index.html");

	std::cout << "Req inside POST: " << _request.getStatus().first << std::endl;
	std::cout << "TARGET: " << target << std::endl;
	if (target == "/")
		root += index;
	else
		root += target;
	std::cout << "OPENING FILE: " << root << std::endl;
	outfile->open("root/test", std::ios_base::app);
	std::cout << _request.getBody() << std::endl;
	if (!outfile->is_open())
	{
		std::cout << "[DEBUG] POSTMethod file creation failed\n";
		delete outfile;
		// set status code
		return _response;
	}
	return _response;
}


