#include <unistd.h>         // chdir, dup, execve, getcwd
#include <sys/types.h>      // umask, fork, setsid, kill
#include <sys/stat.h>       // umask
#include <sys/time.h>       // getrlimit
#include <sys/resource.h>   // getrlimit
#include <signal.h>         // signal, kill
#include <errno.h>          // errno - number of last error
#include <stdlib.h>         // rand, srand
#include <fcntl.h>          // For O_* constants
#include <stdio.h>          // sprintf, printf, fprintf
#include <time.h>           // time

int sigtermReceived;
int sigusrValue;

void handler1(int sig)    // Separate handler functions:
{                         // handler1 for SIGTERM
  sigtermReceived = 1;
}

void handler2(int sig)    // handler2 for SIGUSR1 and SIGUSR2
{
  if(sig == SIGUSR1)
  {
    sigusrValue = 1;
  }
  else if(sig == SIGUSR2)
  {
    sigusrValue = 2;
  }

  signal(sig, handler2);    // the handler registers itself again
}

int main(int argc, char **argv)
{
  pid_t mole1 = 0;
  pid_t mole2 = 0;

  umask(0); // set the file creation mask to 0;
  char buf[50];
  char pathToMole[50];
  getcwd(buf, 50);    /* getcwd - copies an absolute pathname of the current working directory to
                                  the array pointed to by buf, which is of length 50 */
  sprintf(pathToMole, "%s/mole", buf);
  printf("%s\n", pathToMole);
  pid_t process_id = fork();  /*  Creates a new process by duplicating the calling process.
                                  The new process is referred to as the child process.
                                  The calling process is referred to as the parent process. */
  if(process_id == -1)
  {
    fprintf(stderr, "Can't fork, error %d\n", errno);
    exit(EXIT_FAILURE);
  }
  else if(process_id == 0)  // Child process
  {
    pid_t session_id = setsid();  // Creates a session and sets the process group ID

    chdir("/");   /*  changes the current working directory of the calling
                      process to the directory specified by the character "/"   */

    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);  /*  getrlimit - system call that gets resource limit
                                          RLIMIT_NOFILE - specifies a value one greater than the maximum file
                                                          descriptor number that can be opened by this process   */
    int i = 0;
    while(i < (int)rlim.rlim_cur)   // specifies the current or soft limit
    {
      close(i);
      i++;        // Close all unneeded file descriptors
    }

    int file_descriptor = open("/dev/null", O_RDWR);

    dup(file_descriptor);
    dup(file_descriptor);   // Reopen the standard file descriptors to map to "/dev/null"

    signal(SIGTERM, handler1);    // Handler function assignments for SIGTERM, SIGUSR1, and SIGUSR2
    signal(SIGUSR1, handler2);
    signal(SIGUSR2, handler2);

    srand(time(NULL));    // Makes use of the computer's internal clock to control the choice of the seed

    while(sigtermReceived != 1)   // Executes while a SIGTERM has not reached its designated handler
    {
      int randNum;

      pause();

      if(sigusrValue == 1)    // SIGUSR1
      {
        if(mole1 > 0)
        {
          kill(mole1, SIGTERM);   // kill child process #1 (mole 1)
        }

        randNum = rand() % 2 + 1;     // Randomly create either mole 1 or mole 2 if it does not already exists
        if(randNum == 1 && mole1 == 0)
        {
          mole1 = fork();
          if(mole1 == 0)
          {
            char *args[3] = {"mole1", getenv("HOME"), NULL};   /* getenv -  searches the environment list to find the environment variable "HOME",
                                                                            and returns a pointer to the corresponding value string    */

            execve(pathToMole, args, NULL);   // execve - executes the program referred to by pathToMole
          }
        }
        else if (randNum == 2 && mole2 == 0)
        {
          mole2 = fork();
          if(mole2 == 0)
          {
            char *args[3] = {"mole2", getenv("HOME"), NULL};
            execve(pathToMole, args, NULL);
          }
        }

        sigusrValue = 0;
      }

      else if(sigusrValue == 2)   // SIGUSR2
      {
        if(mole2 > 0)
        {
          kill(mole2, SIGTERM);   // kill child process #1 (mole 2)
        }

        randNum = rand() % 2 + 1;   // Randomly create either mole 1 or mole 2 if it does not already exists
        if(randNum == 1 && mole1 == 0)
        {
          mole1 = fork();
          if(mole1 == 0)
          {
            char *args[3] = {"mole1", getenv("HOME"), NULL};
            execve(pathToMole, args, NULL);
          }
        }
        else if (randNum == 2 && mole2 == 0)
        {
          mole2 = fork();
          if(mole2 == 0)
          {
            char *args[3] = {"mole2", getenv("HOME"), NULL};
            execve(pathToMole, args, NULL);   // exec the program mole, with the value of argv[0] set to either mole1 or mole2

          }
        }

        sigusrValue = 0;
      }
    }

    // Upon SIGTERM, the program kills all child processes and shuts down the daemon gracefully

    kill(mole1, SIGTERM);
    kill(mole2, SIGTERM);
    exit(0);

  }
  else
  {
    printf("The process ID is %d\n", process_id);
    exit(0);    // the parent exits after printing out the PID
  }

}
