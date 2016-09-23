#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define	FS_COMPR_FL			0x00000004
#define	FS_IOC_GETFLAGS			_IOR('f', 1, long)
#define	FS_IOC_SETFLAGS			_IOW('f', 2, long)
#define OPEN_FLAGS (O_RDONLY|O_NONBLOCK)
int main(int argc, char* argv[])
{
	int fd;
	int f,r;
	if(0==argc)
	{
		printf("no input file.\n");
		exit(0);
	}
	fd = open(argv[1], OPEN_FLAGS);
	printf("doing:%s\n",argv[1]);
	r = ioctl(fd, FS_IOC_GETFLAGS, &f);
	printf("old:%x\n",f);
	f|=FS_COMPR_FL;
	r = ioctl(fd, FS_IOC_SETFLAGS, &f);
	printf("new:%x\n",f);
	close(fd);
	return 0;
}
