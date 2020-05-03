#include <time.h>            // time
#include <stdio.h>           // printf, sprintf, fprintf
#include <unistd.h>          // usleep
#include <stdlib.h>          // atoi, srand, rand
#include <errno.h>           // errno - number of last error
#include <signal.h>          // signal and signal handling
#include <fcntl.h>           // For O_* constants
#include <sys/stat.h>        // For mode constants
#include <semaphore.h>       /* sem_close, sem_unlink, sem_open, sem_wait, sem_post */

int booleanVar;

void eat(int position)
{
  int randNum;
  randNum = rand() % 3 + 1;     // generates a random number
  usleep(randNum * 1000000);    // each function consumes a random amount of real time
  printf("Philosopher #%d is eating\n", position);
}

void think(int position)
{
  int randNum;
  randNum = rand() % 3 + 1;     //generates a random number
  printf("Philosopher #%d is thinking\n", position);
  usleep(randNum * 1000000);    // each function consumes a random amount of real time
}

void wait(sem_t *semaphore)
{
  /*  decrements (locks) the semaphore pointed to by sem.  If
      the semaphore's value is greater than zero, then the decrement
      proceeds, and the function returns, immediately. */
  sem_wait(semaphore);

  /*  If the semaphore currently has the value zero, then the call
      blocks until either it becomes possible to perform the decrement */

}

void signalFunc(sem_t *semaphore)
{
  /*  increments (unlocks) the semaphore pointed to by sem.  If
      the semaphore's value consequently becomes greater than zero, then
      another process or thread blocked in a sem_wait(3) call will be woken
      up and proceed to lock the semaphore. */

      sem_post(semaphore);

  /*   returns 0 on success; on error, the value of the semaphore
       is left unchanged, -1 is returned, and errno is set to indicate the
       error. */

}

void handler(int sig)
{
  booleanVar = 1;   // boolean variable representing SIGTERM call
}

int main(int argc, char **argv)
{
  int seats = atoi(argv[1]);
  int position = atoi(argv[2]);   // converts a string argument str to an integer (type int)

  char leftName[10];
  char rightName[10];

  if(position < seats)
  {
    sprintf(rightName, "/sem_%d", position + 1);
  }                                                 /*  sprintf writes the data in the string pointed to by str and returns
                                                        the number of characters written to str, excluding the null character */
  else
  {
    sprintf(rightName, "/sem_%d", 1);               // left and right name representing a utensil to each side of the dining philosopher
  }

  sprintf(leftName, "/sem_%d", position);

  sem_t *leftSemaphore = sem_open(leftName, O_CREAT, S_IRWXU | S_IRWXO, 1);
  sem_t *rightSemaphore = sem_open(rightName, O_CREAT, S_IRWXU | S_IRWXO, 1);   // sem_open creates a new POSIX semaphore or opens an existing semaphore

  int cycleCount = 0;

  srand(time(NULL));  //  to prevent sequence repetition between runs

  signal(SIGTERM, handler);   // assigning SIGTERM to a handler

  while(booleanVar != 1) // While SIGTERM has not been caught
  {
    if(position == 1)
    {
      /*  eliminating the possibility of deadlock by removing the circular wait condition.
          the philosopher in position 1 is right handed while the remaining ones are left handed */

      wait(rightSemaphore);
      wait(leftSemaphore);
    }
    else
    {
      wait(leftSemaphore);
      wait(rightSemaphore);   // wait suspends execution of the calling thread until one of its children terminates
    }

    eat(position);
    signalFunc(leftSemaphore);
    signalFunc(rightSemaphore);
    think(position);
    cycleCount++;   // keeping count of number of full eat and think cycles
  }

  // When SIGTERM is caught

  sem_close(leftSemaphore);
  sem_close(rightSemaphore);  // sem_close closes semaphore
  sem_unlink(leftName);
  sem_unlink(rightName);      // sem_unlink removes a named semaphore

  fprintf(stderr,"Philosopher #%d completed %d cycles\n", position, cycleCount);
  exit(0);

}
