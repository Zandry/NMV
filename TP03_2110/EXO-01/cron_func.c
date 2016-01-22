#define _GNU_SOURCE

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<dlfcn.h>

#include"func.h"

#define ATTENTE 1

void (*ptrFunc)(void); 

void handle_sigalrm(int signal)
{
    if (signal == SIGALRM) {
        ptrFunc();
        alarm(ATTENTE);
    }
}

int main(int argc, char const *argv[])
{
    struct sigaction sa;
    char c = 0;
    int tmp;
    void *insertedHandle = NULL, *tmpHandle = NULL;
    char *path = "libanything.so";

    ptrFunc = func;
        
    sa.sa_handler = &handle_sigalrm;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        perror("Error: cannot handle SIGALRM");

    alarm(ATTENTE);

    do {
        if (c == 0)
            printf("Enter : (i) insert (r) remove (e) end\n");

        if (read(STDIN_FILENO, &c, sizeof(char)) != sizeof(char))
            return -1;

        switch (c) {

        case 'i':
            printf("Inserting a new function\n");

            /* Ouvrir la lib si ça n'a pas déjà été fait */
            if(!insertedHandle) {
                insertedHandle = dlopen(path, RTLD_LAZY);
                if(!insertedHandle)
                    printf("Insert : dlopen a renvoyé NULL : %s\n", dlerror());
            }
            
            /* Rechercher le symbole */
            tmpHandle = dlsym(insertedHandle, "func");
            if(!tmpHandle)
                printf("Insert : dlsym a renvoyé NULL : %s\n", dlerror());

            /* Rediriger la fonction */
            ptrFunc = tmpHandle;
                        
            c = 0;
            break;
        case 'r':
            printf("Restoring the default function\n");

            /* Rechercher la fonction initiale */
            tmpHandle = dlsym(RTLD_DEFAULT, "func");
            if(!tmpHandle)
                printf("Restore : dlsym a renvoyé NULL : %s\n", dlerror());

            /* Fermer le descripteur ouvert (s'il l'a été) */
            if(insertedHandle) {
                tmp = dlclose(insertedHandle);
                if(tmp)
                    printf("Restore : dlclose a renvoyé %d : %s\n", tmp, dlerror());
                insertedHandle = NULL;
            }

            /* Rediriger la fonction */
            ptrFunc = tmpHandle;

            c = 0;
            break;
        default:
            break;

        }
    } while (c != 'e');

    return 0;
}
