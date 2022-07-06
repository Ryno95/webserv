#pragma once

#include <config/AParseTreeLeaf.hpp>
#include <config/LocationConfig.hpp>

namespace Webserver
{
	class LocationConfigParser : public AParseTreeLeaf<LocationConfig>
	{
	public:
		LocationConfigParser(StreamData* streamData, const std::string& pattern, const HostFields& parent);
		LocationConfigParser(StreamData* streamData);

		void validate();
		void fillData();
		std::map<std::string, ICommand*> createKeywords();
	private:
		const HostFields* _parentData;
	};
}
