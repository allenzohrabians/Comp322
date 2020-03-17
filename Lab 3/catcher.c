#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main()
{
  __sighandler_t signal(int signum, __sighandler_t handler);

  pause();
}
