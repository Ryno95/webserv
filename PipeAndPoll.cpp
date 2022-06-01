#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <string>

class Pipe
{
    int _fds[2];
public:
    Pipe()
    {
        std::cout << "Piped" << std::endl;
        pipe(_fds);
    }
    int getReadFd()
    {
        return _fds[0];
    }
    int getWriteFd()
    {
        return _fds[1];
    }
};

class Fork
{
    Pipe pipe;
    int _pid;

public:
    Fork()
    {
        std::cout << "Forked" << std::endl;
        _pid = fork();
        if (_pid == -1)
            std::cout << "FORK ERROR" << std::endl;
        
        if (_pid == 0)
            close(pipe.getReadFd());
        else
            close(pipe.getWriteFd());
    }
    bool isChild()
    {
        return _pid == 0;
    }
    int getPipeFd()
    {
        if (isChild())
            return pipe.getWriteFd();
        else
            return pipe.getReadFd();
    }
};

std::vector<pollfd> _fds;

int main()
{
    Fork process;
    pollfd fd;

    if (process.isChild())
    {
        if (dup2(process.getPipeFd(), STDOUT_FILENO) == -1)
			std::cout << "dup2 ERROR" << std::endl;
        execve("/usr/local/bin/python3 hello.py", NULL, NULL);
        std::cout << "EXECVE ERROR" << std::endl;
    }
    pollfd pfd;
    pfd.fd = process.getPipeFd();
    pfd.events = POLLIN;
    _fds.push_back(pfd);

    if (fcntl(process.getPipeFd(), F_SETFL, O_NONBLOCK) == -1)
        std::cout << "fcntl ERROR" << std::endl;

    char buffer[2];
    std::string str;
    while (true)
    {
        int pollret = poll(&_fds.front(), _fds.size(), 1000);
        if (pollret != 0)
        {
            int bytes = read(process.getPipeFd(), buffer, 1);
            if (bytes == -1)
            {
                std::cout << "Blocking..." << std::endl;
            }
            else if (bytes == 0)
            {
                std::cout << "Finished..." << std::endl;
                break;
            }
            else
            {
                buffer[1] = 0;
                std::cout << buffer << std::endl;
                str += buffer;
            }
        }
    }
    std::cout << "FINISHED! Result:\n" << str << std::endl;
}
