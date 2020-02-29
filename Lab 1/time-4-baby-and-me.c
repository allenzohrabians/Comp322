#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */
#include <time.h>   /* clock_t, time_h, struct tm */
#include <sys/wait.h> /* waitpid() */
#include <sys/times.h>

void parentChildReport(pid_t* process_id, int* stat_loc)
{
    pid_t callingPid = getpid();
    pid_t callingParentPid = getppid();
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

    /* Output from both the child and the parent process
     * will be written to the standard output,
     * as they both run at the same time.
     */
    seconds = time(NULL);
    sleep(4);
    printf("START: %ld\n", seconds);
    pid_t  process_id = fork();

    if (process_id == -1)
    {
        /* Error:
         * When fork() returns -1, an error happened
         * (for example, number of processes reached the limit).
         */
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (process_id == 0)
    {
        /* Child process:
         * When fork() returns 0, we are in
         * the child process.
         */
        parentChildReport(NULL, NULL);
        exit(0);  /* Note that we do not use exit() */
    }
    else
    {
        /* When fork() returns a positive number, we are in the parent process
         * (the fork return value is the PID of the newly created child process)
         * Again we count up to ten.
         */
        int stat_loc;
        struct tms buf;
        waitpid(process_id, &stat_loc, 0);
        parentChildReport(&process_id, &stat_loc);

        times(&buf);

        printf("USER: %ld, SYS: %ld\n", buf.tms_utime, buf.tms_stime);
        printf("CUSER: %ld, CSYS: %ld\n", buf.tms_cutime, buf.tms_cstime);
        seconds = time(NULL);
        printf("STOP: %ld\n", seconds);
        exit(0);
    }
    return 0;
}