#include <AMethod.hpp>
#include <Request.hpp>
#include <Response.hpp>

class POSTMethod : public AMethod
{
    public:
        POSTMethod(const Request &request);
        ~POSTMethod();
        Response *process();
};
