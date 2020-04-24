#include <string.h> /* strcmp */
#include <stdlib.h> /* exit */
#include <stdio.h>  /* printf, stderr, fprintf */
#include <signal.h> /* signal */
#include <unistd.h> /* pause */
#include <errno.h>  /* errno */
#include <time.h>  /* time */
#include <sys/types.h>  /* pid_t */

typedef void (*sighandler_t)(int); // pointer-to-function type

// separate arrays for the signals themselves as well as the corresponding strings of the signal names

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

      printf("SIG%s caught at %ld\n", signalStrings[i], seconds);   // prints what signal was caught and at what time

      allSignalCounter++;   // total number of signals caught for final print statement

      if(i == 14) // index for SIGTERM
      {
        sigtermCounter++; // increments after each successive SIGTERM signal
      }
      else
      {
        sigtermCounter = 0; // resets to 0 if SIGTERM is not the signal caught
      }

      break;
    }
    i++;
  }

  signal(sig, handler); // the handler registers itself again
}

int main(int argc, char* argv[])
{
  pid_t process_id;
  process_id = getpid(); // Retrieval and printing of PID
  fprintf(stderr,"catcher: $$ = %d\n", process_id);
  void handler(int);
  int i = 1;
  while(i < argc)
  {
    int j = 0;
    while(j < 27)
    {
      if(strcmp(argv[i], signalStrings[j]) == 0)  // indicates that the two strings are equal
      {
        signal(signalInts[j], handler);         // registers a handler for each argument
        break;
      }
      j++;
    }
    i++;
  }

  while(sigtermCounter != 3)
  {
    pause();  // the program pauses itself continually and terminates once SIGTERM counter = 3
  }

  fprintf(stderr, "catcher: Total signals count = %d\n", allSignalCounter);
  exit(0);

}
