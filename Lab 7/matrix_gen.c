#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
  srand(time(NULL));

  int size = atoi(argv[1]);

  for(int i = 0; i < size*size; i++)
  {
    int randNum = (rand() % 201) - 100;
    printf("%-5d", randNum);
  }
}
