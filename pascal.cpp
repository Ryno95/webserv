#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>


namespace FileType
{
	/*
		DT_BLK      This is a block device.
		DT_CHR      This is a character device.
		DT_DIR      This is a directory.
		DT_FIFO     This is a named pipe (FIFO).
		DT_LNK      This is a symbolic link.
		DT_REG      This is a regular file.
		DT_SOCK     This is a UNIX domain socket.
		DT_UNKNOWN  The file type could not be determined.
	*/
	enum FileType
	{
		BLOCK,
		CHAR,
		DIR,
		FIFO,
		SYMLINK,
		FILE,
		SOCKET,
		UNKNOWN
	};
}

std::vector<std::string> getDirEntries()
{
	dirent* dirEntry;
	DIR* dir;
	std::vector<std::string> entries;

	dir = opendir(".");
	while (true)
	{
		dirEntry = readdir(dir);
		if (dirEntry == nullptr)
			break;
		entries.push_back(dirEntry->d_name);
	}
	closedir(dir);
	return entries;
}



int main()
{
	std::vector<std::string> dirEntries = getDirEntries();
	for (size_t i = 0; i < dirEntries.size(); i++)
	{
		std::cout << dirEntries[i] << std::endl;
	}
}