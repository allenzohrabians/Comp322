#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <sys/wait.h> /* waitpid(), WIFEXITED(), WEXITSTATUS()*/

int main(int argc, char *argv[])
{
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

        execve(argv[1], argv + 1, NULL);
    }
    else
    {
        /* When fork() returns a positive number, we are in the parent process
         (the fork return value is the PID of the newly created child process) */
         int stat_loc;
         fprintf(stderr, "%s: $$ = %d\n", argv[1], process_id);
         waitpid(process_id, &stat_loc, 0);
         if(WIFEXITED(stat_loc) != 0)
         {
           fprintf(stderr, "%s: $? = %d\n", argv[1], WEXITSTATUS(stat_loc));
         }

    }

    return 0;
}
