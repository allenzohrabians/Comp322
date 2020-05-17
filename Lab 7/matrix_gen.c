#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
  srand(time(NULL));    // makes use of the computer's internal clock to control the choice of the seed

  int size = atoi(argv[1]);   // atoi - converts the string argument to an integer

  for(int i = 0; i < size*size; i++)
  {
    int randNum = (rand() % 201) - 100;
    printf("%-5d", randNum);
    // Generates a matrix of size “size * size” that contains a random set of integers from -100 to +100
  }
}
