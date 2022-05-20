#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

enum FDs
{
	READ_FD,
	WRITE_FD
} FDs;

int gofukingmental()
{
    int pipeFd[2];
    int pid;
    int resetFd[2];

    if (pipe(pipeFd) <  0)
			return (write(1, "Pipe() failed\n", 15));

    pid = fork();
	if (pid < 0)
		return (write(1, "Fork() failed\n", 15));
	else if (pid > 0)
	{
		// parent process
		// read here
		close(pipeFd[WRITE_FD]);
		char buffer[100];
        int readBytes = 0, bufferRead = 0;

        while (1)
        {
            readBytes = read(pipeFd[READ_FD], &buffer[bufferRead], 2);
            if (readBytes <=  0)
                break ;
            bufferRead += readBytes;
        }
        buffer[bufferRead] = '\0';
        std::cout <<  buffer << std::endl;
		close(pipeFd[READ_FD]);
	}
	else
	{
		close(pipeFd[READ_FD]);
		if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
			return (write(1, "dup2() failed\n", 15));
        std::cerr << "Executing hello.cpp" << std::endl;
        execve("hello", NULL, NULL);
        std::cerr << "Executed hello.cpp" << std::endl;
		//	child process
		//	write here
	}
}

int main(void)
{
    gofukingmental();
    gofukingmental();
    gofukingmental();
    gofukingmental();
    gofukingmental();
	// wait(NULL);
    // close(1);
    while (true);
    return (0);
}