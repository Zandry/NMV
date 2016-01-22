 #define _GNU_SOURCE
       #include <unistd.h>
       #include <sys/syscall.h>
       #include <sys/types.h>
       #include <signal.h>
 #include <stdlib.h>
 #include <stdio.h>

       int main(int argc, char *argv[])
       {
           pid_t pid = atoi(argv[1]);
           pid_t tid = atoi(argv[2]);
           printf("Trying to kill pid=%d, tid=%d\n", pid, tid);
           tid = syscall(SYS_tgkill, pid, tid, SIGKILL);
           printf("%d\n", tid);
           return 1;
       }
