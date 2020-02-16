#include <stdio.h>
#include <math.h>
#include <stdlib.h>

decimalConversion(binaryArr[])
{
  int decimalVal = 0;

  for (int i = 7, z = 0; i > 0; i--, j++)
  {
    decimalVal = decimalVal + ((binaryArr[i] - 48) * pow(2,z));
  }

  return decimalVal;
}

char* parity(binaryArr[])
{
  int sumPar = 0;

  for (int i = 0; i < 8; i++)
  {
    sumPar += binaryArr[i];
  }

  if(sumPar % 2 == 0)
  {
    return "EVEN";
  }

  else
  {
    return "ODD";
  }
}

int main(int argc, char** argv)
{
  int reader;
  int binaryVal[8] = {0};

  if (argc < 2)
  {
    printf("Sorry, file does not have a valid name.\n");
  }

  else
  {
    printf("Original ASCII    Decimal  Parity\n");
    printf("-------- -------- -------- --------\n");

    FILE* file = fopen(argv[1], "r");

    int argCounter = 0;

    while((reader = fgetc(file)) != EOF)
    {
      if (reader != ' ')
      {
        binaryVal[argCounter] = reader;
        argCounter++;
      }

      if (argCounter == 8 || (argCounter > 0 && reader == " "))
      {
        for (int i = 0; i < 8; i++)
        {
          printf("%c", binaryVal[i]);
        }
        printf("\t");

        printf("%c", decimalConversion(binaryVal));

        printf("%8d\t", decimalConversion(binaryVal));

        printf("%s", parity(binaryVal));

        for (int i = 0; i < 8; i++)
        {
          binaryVal[i] = '0';
        }

        argCounter = 0;

        printf("\n");
      }
    }

    if (argCounter > 0)
    {
      for (int j = counter - 1; j < 8; j++)
      {
        binaryVal[j] = '0';
      }

      for (int i = 0; i < 8; i++)
      {
        printf("%c", binaryVal[i]);
      }
      printf("\t");

      printf("%c", decimalConversion(binaryVal));

      printf("%8d\t", decimalConversion(binaryVal));

      printf("%s", parity(binaryVal));

      printf("\n");

      argCounter = 0;
    }

    fclose(file);
  }

  return 0;
}
