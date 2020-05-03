#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  const int PAGE_SIZE = 4096;   // Assume that a system has a 32-bit virtual address with a 4-KB page size
  unsigned int address;
  unsigned int page_number;
  unsigned int offset;

  if(argc < 2)
  {
    printf("Invalid number of arguments.\n");
    return -1;
  }

  address = atoi(argv[1]);

  page_number = address / PAGE_SIZE;

  offset = address % PAGE_SIZE;

  printf("The address %d contains: \n", address);
  printf("Page number = %d \n", page_number);
  printf("Offset = %d \n", offset);

  return 0;
}
