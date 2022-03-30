#include <criterion/criterion.h>
#include <Request.hpp>

Test(simple, valid)
{
	cr_expect(1 == 1);
}

// Test(requestLineTests, INVALID_METHOD)
// {
// 	std::string seed = "BLADIEBLA / HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_valid_request)
// {
// 	std::string seed = "GET / HTTP/1.1\r\n";
// 	Request request(seed);

// 	cr_expect(request._method == GET);
// 	cr_expect(request._target == "/");
// }

// Test(requestLineTests, GET_valid_request_large_path)
// {
// 	std::string seed = "GET /fjadsffkjadskljffffjadsffkjadskljfffdsfadsjfjdjsfkldjfadjslfkjdsfadsjfjdjsfkldjfadjslfkj HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	if (status == URI_TOO_LONG)
// 		return;
// 	cr_expect(request._method == GET);
// 	cr_expect(request._target == "/fjadsffkjadskljffffjadsffkjadskljfffdsfadsjfjdjsfkldjfadjslfkjdsfadsjfjdjsfkldjfadjslfkj");
// }

// Test(requestLineTests, POST_valid_request)
// {
// 	std::string seed = "POST / HTTP/1.1\r\n";
// 	Request request(seed);

// 	cr_expect(request._method == POST);
// 	cr_expect(request._target == "/");
// }

// Test(requestLineTests, DELETE_valid_request)
// {
// 	std::string seed = "DELETE / HTTP/1.1\r\n";
// 	Request request(seed);

// 	cr_expect(request._method == DELETE);
// 	cr_expect(request._target == "/");
// }

// Test(requestLineTests, DELETE_no_target)
// {
// 	std::string seed = "DELETE  HTTP/1.1\r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_no_version)
// {
// 	std::string seed = "DELETE / \r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_no_version2)
// {
// 	std::string seed = "DELETE /\r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_too_many_whitespace1)
// {
// 	std::string seed = "DELETE  / HTTP/1.1\r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_too_many_whitespace2)
// {
// 	std::string seed = "DELETE /  HTTP/1.1\r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, empty_seed)
// {
// 	std::string seed;
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_leading_whitespace)
// {
// 	std::string seed = " DELETE / HTTP/1.1\r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, DELETE_trailing_whitespace)
// {
// 	std::string seed = "DELETE / HTTP/1.1 \r\n";
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, target_just_fits)
// {
// 	std::string seed = "GET ";
// 	for (size_t i = 0; i < MAX_TARGET_LEN; ++i)
// 	{
// 		seed += "a";
// 	}
// 	seed += " HTTP/1.1\r\n";
	
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == OK);
// }


// Test(requestLineTests, target_just_too_long)
// {
// 	std::string seed = "GET ";
// 	for (size_t i = 0; i <= MAX_TARGET_LEN; ++i)
// 	{
// 		seed += "a";
// 	}
// 	seed += " HTTP/1.1\r\n";
	
// 	Request request(seed);
// 	httpStatusCode status = request.parseRequestLine();

// 	cr_expect(status == URI_TOO_LONG);
// }

// Test(requestLineTests, GET_invalid_version1)
// {
// 	std::string seed = "GET / HTTP/a1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_invalid_version2)
// {
// 	std::string seed = "GET / HTTP/1a.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_invalid_version3)
// {
// 	std::string seed = "GET / HTTP/1.a1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_invalid_version4)
// {
// 	std::string seed = "GET / HTTP/1.1a\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_invalid_version5)
// {
// 	std::string seed = "GET / HTTP/1.\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_invalid_version6)
// {
// 	std::string seed = "GET / HTTP/.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_redundant_zeros)
// {
// 	std::string seed = "GET / HTTP/00000001.000000000000001\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == OK);
// }

// Test(requestLineTests, GET_CR_in_target)
// {
// 	std::string seed = "GET /\r HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_LF_in_target)
// {
// 	std::string seed = "GET /\n HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_CRLF_in_target)
// {
// 	std::string seed = "GET /\r\n HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }

// Test(requestLineTests, GET_LFCR_in_target)
// {
// 	std::string seed = "GET /\n\r HTTP/1.1\r\n";
// 	Request request(seed);

// 	httpStatusCode status = request.parseRequestLine();
// 	cr_expect(status == BAD_REQUEST);
// }
