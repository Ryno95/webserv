#include <criterion/criterion.h>
#include <Request.hpp>

Test(requestLineTests, GET_valid_request)
{
	std::string seed = "GET / http/1.1\r\n";
	Request request(seed);

	cr_expect(request._method == GET);
	cr_expect(request._target == "/");
	cr_expect(request._version == "http/1.1");
}

Test(requestLineTests, GET_valid_request_large_path)
{
	std::string seed = "GET /fjadsffkjadskljffffjadsffkjadskljfffdsfadsjfjdjsfkldjfadjslfkjdsfadsjfjdjsfkldjfadjslfkj http/1.1\r\n";
	Request request(seed);

	httpStatusCode status = request.parseRequestLine();
	if (status == URI_TOO_LONG)
		return;
	cr_expect(request._method == GET);
	cr_expect(request._target == "/fjadsffkjadskljffffjadsffkjadskljfffdsfadsjfjdjsfkldjfadjslfkjdsfadsjfjdjsfkldjfadjslfkj");
	cr_expect(request._version == "http/1.1");
}

Test(requestLineTests, POST_valid_request)
{
	std::string seed = "POST / http/1.1\r\n";
	Request request(seed);

	cr_expect(request._method == POST);
	cr_expect(request._target == "/");
	cr_expect(request._version == "http/1.1");
}

Test(requestLineTests, DELETE_valid_request)
{
	std::string seed = "DELETE / http/1.1\r\n";
	Request request(seed);

	cr_expect(request._method == DELETE);
	cr_expect(request._target == "/");
	cr_expect(request._version == "http/1.1");
}

Test(requestLineTests, DELETE_no_target)
{
	std::string seed = "DELETE  http/1.1\r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_no_version)
{
	std::string seed = "DELETE / \r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_no_version2)
{
	std::string seed = "DELETE /\r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_too_many_whitespace1)
{
	std::string seed = "DELETE  / http/1.1\r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_too_many_whitespace2)
{
	std::string seed = "DELETE /  http/1.1\r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, empty_seed)
{
	std::string seed;
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_leading_whitespace)
{
	std::string seed = " DELETE / http/1.1\r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, DELETE_trailing_whitespace)
{
	std::string seed = "DELETE / http/1.1 \r\n";
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == BAD_REQUEST);
}

Test(requestLineTests, target_just_fits)
{
	std::string seed = "GET ";
	for (size_t i = 0; i < MAX_TARGET_LEN; ++i)
	{
		seed += "a";
	}
	seed += " http/1.1\r\n";
	
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == OK);
}


Test(requestLineTests, target_just_too_long)
{
	std::string seed = "GET ";
	for (size_t i = 0; i <= MAX_TARGET_LEN; ++i)
	{
		seed += "a";
	}
	seed += " http/1.1\r\n";
	
	Request request(seed);
	httpStatusCode status = request.parseRequestLine();

	cr_expect(status == URI_TOO_LONG);
}
