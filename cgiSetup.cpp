#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <stdlib.h>

enum FDs
{
	READ_FD,
	WRITE_FD
} FDs;

int gofukingmental(char *argv)
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
        wait(NULL);
	}
	else
	{
		//	child process
		close(pipeFd[READ_FD]);
		if (dup2(pipeFd[WRITE_FD], STDOUT_FILENO) < 0)
			return (write(1, "dup2() failed\n", 15));

        std::cerr << "Executing hello.cpp" << std::endl;

        char queryString[] = "QUERY_STRING=val1=5&val2=66";

        if (putenv(queryString) == -1)
            std::cout << "Setenv failed!" << std::endl;

        std::cerr << "Does this return what i need?" << getenv("QUERY_STRING") << std::endl;
        
        const char *argv[] = {"env", "-i", queryString, "/usr/local/bin/python3", "root/cgi-bin/add.py", NULL};

        // if (system("python3 root/cgi-bin/add.py") == -1)
        //     perror("");
        const char *path = "/usr/bin/env";
        if (execve(path, (char *const *)argv,NULL) == -1)
            perror("");
        close(pipeFd[WRITE_FD]);
        std::cerr << "Executed hello.cpp" << std::endl;
	}
    return (0);
}

int main(int argc, char *argv[])
{
    gofukingmental(argv[1]);
    while (true);
    return (0);
}