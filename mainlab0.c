#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
  int binaryVal;
  char asciiVal;
  int decimalVal;
  bool evenVal;

   FILE *fptr;
   if ((fptr = fopen("C:\\Users\allen\Desktop\Comp322\testdata.txt","r")) == NULL){
       printf("Error! opening file");
       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   fscanf(fptr,"%d", &binaryVal);
   printf("Value of n=%d", binaryVal);
 }
