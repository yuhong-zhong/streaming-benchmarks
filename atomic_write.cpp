#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>
#include <mutex>
#include <deque>
#include <climits>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

using namespace std;


int file_write_lock(int fd)
{
	struct flock fl;

	memset(&fl, 0, sizeof(fl));
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = 0;

	return fcntl(fd, F_SETLKW, &fl);
}

int file_write_unlock(int fd)
{
	struct flock fl;

	memset(&fl, 0, sizeof(fl));
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = 0;

	return fcntl(fd, F_SETLK, &fl);
}

/*
 * Main Function
 */

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Usage: <value> <path>" << endl;
	}

	int fd = open(argv[2], O_WRONLY | O_CREAT, 00777);
	if (fd < 0) {
		cout << "cannot open file" << endl;
		return -1;
	}

	int ret = file_write_lock(fd);
	if (ret < 0) {
		cout << "cannot lock file" << endl;
		exit(1);
	}

	char extra[2];
	extra[0] = ' ';
	extra[1] = 0;
	write(fd, argv[1], strlen(argv[1]));
	write(fd, extra, 2);

	file_write_unlock(fd);
}
