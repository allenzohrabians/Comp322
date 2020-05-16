#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char path[100];
  sprintf(path, "%s/lab6.log", argv[1]);
  FILE *logName = fopen(path, "w+");
  fprintf(logName, "Pop %s\n", argv[0]);
  fclose(logName);
}
