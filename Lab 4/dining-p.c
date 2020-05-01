#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

int booleanVar;

void eat(int position)
{
  int randNum;
  randNum = rand() % 3 + 1;
  usleep(randNum * 1000000);
  printf("Philosopher #%d is eating\n", position);
}

void think(int position)
{
  int randNum;
  randNum = rand() % 3 + 1;
  printf("Philosopher #%d is thinking\n", position);
  usleep(randNum * 1000000);
}

void wait(sem_t *semaphore)
{
  sem_wait(semaphore);
}

void signalFunc(sem_t *semaphore)
{
  sem_post(semaphore);
}

void handler(int sig)
{
  booleanVar = 1;
}

int main(int argc, char **argv)
{
  int seats = atoi(argv[1]);
  int position = atoi(argv[2]);

  char leftName[10];
  char rightName[10];

  if(position < seats)
  {
    sprintf(rightName, "/sem_%d", position + 1);
  }
  else
  {
    sprintf(rightName, "/sem_%d", 1);
  }

  sprintf(leftName, "/sem_%d", position);

  sem_t *leftSemaphore = sem_open(leftName, O_CREAT, S_IRWXU | S_IRWXO, 1);
  sem_t *rightSemaphore = sem_open(rightName, O_CREAT, S_IRWXU | S_IRWXO, 1);
  int cycleCount = 0;
  srand(time(NULL));
  signal(SIGTERM, handler);
  while(booleanVar != 1)
  {
    if(position == 1)
    {
      wait(rightSemaphore);
      wait(leftSemaphore);
    }
    else
    {
      wait(leftSemaphore);
      wait(rightSemaphore);
    }

    eat(position);
    signalFunc(leftSemaphore);
    signalFunc(rightSemaphore);
    think(position);
    cycleCount++;
  }

  sem_close(leftSemaphore);
  sem_close(rightSemaphore);
  sem_unlink(leftName);
  sem_unlink(rightName);

  fprintf(stderr,"Philosopher #%d completed %d cycles\n", position, cycleCount);
  exit(0);

}
