#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

int sigtermReceived;
int sigusrValue;

void handler1(int sig)
{
  sigtermReceived = 1;
}

void handler2(int sig)
{
  if(sig == SIGUSR1)
  {
    sigusrValue = 1;
  }
  else if(sig == SIGUSR2)
  {
    sigusrValue = 2;
  }

  signal(sig, handler2);
}

int main(int argc, char **argv)
{
  pid_t mole1 = 0;
  pid_t mole2 = 0;

  umask(0);
  char buf[50];
  char pathToMole[50];
  getcwd(buf, 50);
  sprintf(pathToMole, "%s/mole", buf);
  printf("%s\n", pathToMole);
  pid_t process_id = fork();
  if(process_id == -1)
  {
    fprintf(stderr, "Can't fork, error %d\n", errno);
    exit(EXIT_FAILURE);
  }
  else if(process_id == 0)
  {
    pid_t session_id = setsid();
    chdir("/");
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    int i = 0;
    while(i < (int)rlim.rlim_cur)
    {
      close(i);
      i++;
    }

    int file_descriptor = open("/dev/null", O_RDWR);

    dup(file_descriptor);
    dup(file_descriptor);

    signal(SIGTERM, handler1);
    signal(SIGUSR1, handler2);
    signal(SIGUSR2, handler2);

    srand(time(NULL));

    while(sigtermReceived != 1)
    {
      int randNum;

      pause();

      if(sigusrValue == 1)
      {
        if(mole1 > 0)
        {
          kill(mole1, SIGTERM);
        }

        randNum = rand() % 2 + 1;
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
            execve(pathToMole, args, NULL);
          }
        }

        sigusrValue = 0;
      }

      else if(sigusrValue == 2)
      {
        if(mole2 > 0)
        {
          kill(mole2, SIGTERM);
        }

        randNum = rand() % 2 + 1;
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
            execve(pathToMole, args, NULL);
          }
        }

        sigusrValue = 0;
      }
    }

    kill(mole1, SIGTERM);
    kill(mole2, SIGTERM);
    exit(0);

  }
  else
  {
    printf("The process ID is %d\n", process_id);
    exit(0);
  }

}
