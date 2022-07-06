#include <cgi/Pipes.hpp>
#include <defines.hpp>
#include <unistd.h>
#include <Exception.hpp>

namespace Webserver
{
     Pipes::Pipes()
    {
        serverToCgi[READ_FD] = SYSTEM_ERR;
        serverToCgi[WRITE_FD] = SYSTEM_ERR;

        CgiToServer[READ_FD] = SYSTEM_ERR;
        CgiToServer[WRITE_FD] = SYSTEM_ERR;
    }
    
    Pipes::~Pipes()
    {
        tryClose(serverToCgi[READ_FD]);
        tryClose(serverToCgi[WRITE_FD]);
        
        tryClose(CgiToServer[READ_FD]);
        tryClose(CgiToServer[WRITE_FD]);
    }

    void	Pipes::tryClose(int fd)
    {
        if (fd != SYSTEM_ERR)
            close(fd);
        fd = SYSTEM_ERR;
    }

    void Pipes::closeForParent()
    {
        tryClose(CgiToServer[WRITE_FD]);
        tryClose(serverToCgi[READ_FD]);
    }
    
    void Pipes::closeForChild()
    {
        tryClose(CgiToServer[READ_FD]);
        tryClose(serverToCgi[WRITE_FD]);
    }
    
    void	Pipes::openPipes()
    {
        if (pipe(serverToCgi) < 0 || pipe(CgiToServer) < 0)
            throw InvalidRequestException(HttpStatusCodes::INTERNAL_ERROR);
    }

}
