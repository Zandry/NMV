 #define _GNU_SOURCE
 #include <unistd.h>
 #include <stdlib.h>
 #include <stdio.h>
       #include <sys/syscall.h>
       #include <sys/types.h>
       #include <signal.h>

int main(int argc, char *argv[]){
   /*while(1){
		printf("Hello world! + pid=%d, tid=%d\n", getpid(),  syscall(SYS_gettid));
		sleep(1);
	}*/
	syscall(sys_hello);
	return 1;
}
