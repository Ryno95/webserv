#pragma once

#include <Request.hpp>

namespace Webserver
{
	class RequestParser : private Request
	{
	public:
		RequestParser();
		~RequestParser();

		Request parse(const std::string& query);

	private:
		size_t 	parseRequestLine();
		size_t 	parseRequestMethod();
		size_t 	parseUri(size_t pos);
		size_t 	parseVersion(size_t pos);
		void	validate() const;

		std::string _rawQuery;
	};
}
