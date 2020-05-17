#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char **argv)
{
  // This program writes a signal line to the lab6.log file

  char path[100];
  sprintf(path, "%s/lab6.log", argv[1]);
  FILE *logName = fopen(path, "w+");
  fprintf(logName, "Pop %s\n", argv[0]);  // prints either "Pop mole1" or "Pop mole2"
  fclose(logName);
}
