#include <stdio.h>   /* printf, stderr, fprintf */
#include <string.h> /* strcmp */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork, dup2 */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <sys/wait.h> /* waitpid */

int main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);   /* creates pipe - unidirectional data channel that
                      can be used for interprocess communication */
    int i = 0;
    int posSecondArg;

    while(i < argc)
    {
        if(strcmp(argv[i] , ",") == 0)  // compares two strings character by character.
        {                               // returns 0 if both strings are identical.
            argv[i] = NULL;
            posSecondArg = i + 1;
            break;
        }

        i++;
    }

    pid_t  process_id = fork(); // first fork call

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
        // Child 1

        dup2(pipefd[1], 1); // duplicates an open file descriptor

        execve(argv[1], argv + 1, NULL);  /* causes the program that is currently running
                                            to be replaced by a new program */
    }
    else
    {
      /* When fork() returns a positive number, we are in the parent process
       (the fork return value is the PID of the newly created child process) */

        pid_t process_id2 = fork(); // second fork call

        if (process_id2 > 0)
        {

             fprintf(stderr, "%s: $$ = %d\n", argv[1], process_id);
             fprintf(stderr, "%s: $$ = %d\n", argv[4], process_id2);

        }

        else if (process_id2 == 0)
        {
            // Child 2

            dup2(pipefd[0], 0);
            execve(argv[posSecondArg], argv + posSecondArg, NULL);

            exit(0);
        }

        close(pipefd[1]);
        close(pipefd[0]); // closing both ends of the pipe

         int stat_loc;
         int stat_loc2;

         waitpid(process_id, &stat_loc, 0);
         waitpid(process_id2, &stat_loc2, 0);

         fprintf(stderr, "%s: $? = %d\n", argv[1], stat_loc);
         fprintf(stderr, "%s: $? = %d\n", argv[1], stat_loc2);
    }
    return 0;
}
