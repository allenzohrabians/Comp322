#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <time.h>   /* clock_t, time_h, struct tm */
#include <sys/wait.h> /* waitpid() */
#include <sys/times.h> /* time_t, suseconds_t, tv_sec, tv_usec */

void parentChildReport(pid_t* process_id, int* stat_loc)    /* This function makes a call to get the PID and PPID, */
{                                                           /* then prints the required values checking to see that */
    pid_t callingPid = getpid();                            /* the values of the process ID and status location are null */
    pid_t callingParentPid = getppid();                     /* for the child, else otherwise to include the child process ID and the return value*/
    if(process_id == NULL && stat_loc == NULL)
    {
        printf("PPID: %d, PID: %d\n", callingParentPid, callingPid );
    } else{
        printf("PPID: %d, PID: %d, CPID: %d, RETVAL: %u\n", callingParentPid, callingPid, *process_id, *stat_loc);
    }
}

int main(void)
{
    time_t seconds;

    seconds = time(NULL);
    sleep(4);   // sleep() isn't necessary but I thought I'd include it so that the start and stop times aren't identical
    printf("START: %ld\n", seconds);
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

        parentChildReport(NULL, NULL);
        exit(0);
    }
    else
    {
        /* When fork() returns a positive number, we are in the parent process
         (the fork return value is the PID of the newly created child process) */

        int stat_loc;
        struct tms buf;
        waitpid(process_id, &stat_loc, 0);  // The wait() system call suspends execution of the calling process until one of its children terminates.
        parentChildReport(&process_id, &stat_loc);

        times(&buf);    // The times() function shall fill the tms structure pointed to by buffer with time-accounting information.

        printf("USER: %ld, SYS: %ld\n", buf.tms_utime, buf.tms_stime);
        printf("CUSER: %ld, CSYS: %ld\n", buf.tms_cutime, buf.tms_cstime);
        seconds = time(NULL);
        printf("STOP: %ld\n", seconds);
        exit(0);
    }
    return 0;
}
