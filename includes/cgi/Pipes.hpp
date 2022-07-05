#pragma once


namespace Webserver
{
    enum FDs
	{
		READ_FD,
		WRITE_FD
	};

    class Pipes
        {
            public:
                Pipes();
                ~Pipes();

                void closeForParent();
                void closeForChild();
                void tryClose(int fd);
                void openPipes();

                int	serverToCgi[2];
                int	CgiToServer[2];
        };
}
