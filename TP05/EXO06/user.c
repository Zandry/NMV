#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define GET_SAMPLE _IOR('N',0,char*)
#define TASKMON_STOP _IOR('N',1,void*)
#define TASKMON_START _IOR('N',2,void*)
#define TASKMON_SET_PID _IOW('N',3,int)

int main(int argc, char ** argv)
{
	int f = open("/dev/exo06", O_RDONLY);
	char response[1024];
	
	while (1)
	{
		ioctl(f, GET_SAMPLE, (unsigned long)response);
		printf("%s\n", response);
		sleep(5);
	}
	return 0;
}
