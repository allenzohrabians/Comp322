#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  const int PAGE_SIZE = 4096;   //4-KB page size (stored as a constant integer)
  unsigned int address;
  unsigned int page_number;
  unsigned int offset;      // In 32-bit integers, an unsigned integer has a range of 0 to 2^(32)-1 or about 4 billion

  if(argc != 2)
  {
    printf("Invalid number of arguments.\n");
    return -1;      // The only arguments for the command line are the file name and the decimal virtual address
  }

  address = atoi(argv[1]);    // converts a string argument str to an integer (type int)

  page_number = address / PAGE_SIZE;

  offset = address % PAGE_SIZE;

  printf("The address %d contains: \n", address);
  printf("page number = %d \n", page_number);
  printf("offset = %d \n", offset);

  return 0;
}
