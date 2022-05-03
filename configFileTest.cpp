#include <iostream>
#include <vector>

#include "config/ConfigFileParser.hpp"

int main()
{
	ConfigFileParser parser("config/default.config");
	try
	{
		std::vector<ServerConfig> result = parser.parse();

		for (size_t i = 0; i < result.size(); i++)
		{
			for (size_t j = 0; j < result[i].hosts.size(); j++)
			{
				std::cout << "Names: " << result[i].hosts[j].names.size() << std::endl;
				for (size_t k = 0; k < result[i].hosts[j].names.size(); k++)
				{
					std::cout << result[i].hosts[j].names[k] << std::endl;
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		// std::cout << e.what() << std::endl;
	}
}
