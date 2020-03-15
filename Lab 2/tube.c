#include <stdio.h>   /* printf, stderr, fprintf */
#include <string.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork, dup2 */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <sys/wait.h> /* waitpid() */

int main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);
    int i = 0;
    int posSecondArg;

    //close(pipefd[1]);
    //close(pipefd[0]);
    /* The goal: not have the 4 in the second child. Instead a variable. THe 4 will change. We want the number to be the index of the first element that comes after the comma. Not the value or the pointer but the index. Hint: we have a while loop  that we can */
    while(i < argc)
    {
        if(strcmp(argv[i] , ",") == 0)
        {
            argv[i] = NULL;
            posSecondArg = i + 1;
            break;
        }

        i++;
    }

    pid_t  process_id = fork();

    if (process_id == -1)
    {
        /* Error:
         * When fork() returns -1, an error happened
         * (for example, number of processes reached the limit). */
        fprintf(stderr, "Can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (process_id == 0)
    {
        /* Child process:
         When fork() returns 0, we are in the child process */
        dup2(pipefd[1], 1);
        execve(argv[1], argv + 1, NULL);
    }
    else
    {
        pid_t process_id2 = fork();
        if (process_id2 > 0)
        {
             fprintf(stderr, "%s: $$ = %d\n", argv[1], process_id);
             fprintf(stderr, "%s: $$ = %d\n", argv[4], process_id2);
        }
        else if (process_id2 == 0)
        {
            dup2(pipefd[0], 0);
            execve(argv[posSecondArg], argv + posSecondArg, NULL);

            exit(0);
        }

        close(pipefd[1]);
        close(pipefd[0]);

        /* When fork() returns a positive number, we are in the parent process
         (the fork return value is the PID of the newly created child process) */
         int stat_loc;
         int stat_loc2;
         waitpid(process_id, &stat_loc, 0);
         waitpid(process_id2, &stat_loc2, 0);
         fprintf(stderr, "%s: $? = %d\n", argv[1], stat_loc);
         fprintf(stderr, "%s: $? = %d\n", argv[1], stat_loc2);
    }
    return 0;
}
