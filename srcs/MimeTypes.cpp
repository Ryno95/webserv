#include <MimeTypes.hpp>

#include <Logger.hpp>

MimeTypes::MimeTypes() : _filePath("config/default.mime")
{
	parse();
}

MimeTypes::MimeTypes(const std::string& filePath) : _filePath(filePath)
{
	parse();
}

MimeTypes::~MimeTypes()
{
}

void MimeTypes::parse()
{
	std::ifstream stream(_filePath);
	if (stream.fail())
		throw std::runtime_error("Failed to open MIME types file");
	
	std::string line;
	while (getline(stream, line))
	{
		std::pair<std::string, std::string> mime;
		size_t pos;
		size_t pos2;

		// Get the MIME field first
		pos = line.find(' ');
		if (pos == std::string::npos || pos == 0)
		{
			WARN("Invalid MIME field: " << line);
			continue;
		}

		mime.second = line.substr(0, pos);

		// Skip whitespaces after MIME field
		while (line[pos] == ' ' && pos < line.size())
			pos++;
		
		// If there's no extension after the MIME field
		if (pos == line.size())
		{
			WARN("Invalid MIME field: " << line);
			continue;
		}

		// Add extension(s) for the MIME field to the map
		while (true)
		{
			pos2 = line.find(' ', pos);
			if (pos2 == std::string::npos)
				pos2 = line.size();

			mime.first = line.substr(pos, pos2 - pos);
			_types.insert(mime);

			pos = pos2;

			// Skip whitespaces after extension
			while (line[pos] == ' ' && pos < line.size())
				pos++;

			if (pos == line.size())
				break;
		}
	}
}

const std::string& MimeTypes::getMIMEType(const std::string& extension) const
{
	std::map<const std::string, const std::string>::const_iterator result = _types.find(extension);
	if (result == _types.end())
		throw std::runtime_error("MIME type for this file extension is not contained");
	return result->second;
}
