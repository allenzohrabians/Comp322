#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>

typedef void (*sighandler_t)(int);

char signalStrings[27][10] = {"HUP", "INT", "QUIT", "ILL", "TRAP", "ABRT", "IOT", "BUS", "FPE", "USR1", "SEGV", "USR2", "PIPE", "ALRM", "TERM", "STKFLT", "CHLD", "CONT", "TSTP", "TTIN", "TTOU","URG", "XCPU", "XFSZ", "VTALRM", "PROF", "WINCH"};
int signalInts[27] = {SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGIOT, SIGBUS, SIGFPE, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGSTKFLT, SIGCHLD, SIGCONT, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH};
int sigtermCounter = 0;
int allSignalCounter = 0;

void handler(int sig)
{
  time_t seconds;
  seconds = time(NULL);

  int i = 0;
  while(i < 27)
  {
    if(sig == signalInts[i])
    {

      printf("SIG%s caught at %ld\n", signalStrings[i], seconds);

      allSignalCounter++;

      if(i == 14)
      {
        sigtermCounter++;
      }
      else
      {
        sigtermCounter = 0;
      }

      break;
    }
    i++;
  }

  signal(sig, handler);
}

int main(int argc, char* argv[])
{
  pid_t process_id;
  process_id = getpid();
  fprintf(stderr,"catcher: $$ = %d\n", process_id);
  void handler(int);
  int i = 1;
  while(i < argc)
  {
    int j = 0;
    while(j < 27)
    {
      if(strcmp(argv[i], signalStrings[j]) == 0)
      {
        signal(signalInts[j], handler);
        break;
      }
      j++;
    }
    i++;
  }

  while(sigtermCounter != 3)
  {
    pause();
  }

  fprintf(stderr, "catcher: Total signals count = %d\n", allSignalCounter);
  exit(0);

}
