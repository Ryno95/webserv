#include <POSTMethod.hpp>
#include <fstream>


POSTMethod::POSTMethod(const Request &request) : AMethod(request) {}

POSTMethod::~POSTMethod() {}

Response* POSTMethod::process()
{
	const std::string   &fileName = _request.getTarget();
	std::ofstream       *outfile = new std::ofstream();
	std::string         root("root/");

	if (fileName == "/")
		outfile->open(root + "index.html", std::ios_base::app);
	else
		outfile->open(root + fileName, std::ios_base::app);
		
	if (!outfile->is_open())
	{
		std::cout << "[DEBUG] POSTMethod file creation failed\n";
		delete outfile;
		return _response;
	}

	
	// parse the body first
	// read body into file from request

	return _response;
}


