#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define HELLO _IOR('N',0,char*)
#define WHO _IOR('N',1,char*)

int main(int argc, char ** argv)
{
	int f = open("/dev/hello", O_RDONLY);
	char request[1024];
	char response[1024];
	
	scanf("%s", request);
	
	ioctl(f, WHO, (unsigned long)request);
	
	ioctl(f, HELLO, (unsigned long)response);
	
	printf("%s\n", response);
	return 0;
}
