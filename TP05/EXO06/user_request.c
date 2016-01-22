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
	char request[1024];
	char response[1024];
	unsigned long pid = 0;
	
	while (1)
	{
		scanf("%s", request);
		
		if (strncmp(request, "get", 3) == 0)
		{
			ioctl(f, GET_SAMPLE, (unsigned long)response);
			printf("%s\n", response);
		}
		else if (strncmp(request, "stop", 4) == 0)
			ioctl(f, TASKMON_STOP, NULL);
		else if (strncmp(request, "start", 5) == 0)
			ioctl(f, TASKMON_START, NULL);
		else if (strncmp(request, "setpid", 6) == 0)
		{
			scanf("%d", &pid);
			ioctl(f, TASKMON_SET_PID, pid);
		}
		else if (strncmp(request, "quit", 4) == 0)
			break;
	}
	printf("Have a nice day and See you next time!");
	return 0;
}
