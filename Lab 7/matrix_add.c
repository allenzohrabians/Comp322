#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <aio.h>
#include <errno.h>

void matrix_add(char *buf, int size, int scalar)
{
  int intSize = 5;

  for(int i = 0; i < size*size*intSize; i+=intSize)
  {
    int x = 0;
    char newBuf[5];
    char outputBuf[12];

    newBuf[0] = buf[i];
    newBuf[1] = buf[i+1];
    newBuf[2] = buf[i+2];
    newBuf[3] = buf[i+3];
    newBuf[4] = buf[i+4];

    x = atoi(newBuf);

    x += scalar;

    fprintf(stderr, "x = %d\n", x);

    sprintf(outputBuf, "%-5d", x);

    //memcpy(buf + i, outputBuf, intSize);

    buf[i] = outputBuf[0];
    buf[i+1] = outputBuf[1];
    buf[i+2] = outputBuf[2];
    buf[i+3] = outputBuf[3];
    buf[i+4] = outputBuf[4];
  }
}

int main(int argc, char **argv)
{
  int intSize = 5;

  srand(time(NULL));

  int size = atoi(argv[1]);
  int blocks = atoi(argv[2]);

  if(size % blocks != 0)
  {
    fprintf(stderr, "Error. Blocks doesn't divide evenly into size.\n");
    exit(0);
  }

  int block_size = size / blocks;
  char buf1[block_size*block_size*intSize];
  char buf2[block_size*block_size*intSize];
  char buf3[block_size*block_size*intSize];

  time_t start_time = time(NULL);

  struct aiocb blockRead;
  struct aiocb blockWrite;

  memset(&blockRead, 0, sizeof(struct aiocb));
  blockRead.aio_fildes = 0;
  blockRead.aio_offset = 0;
  blockRead.aio_buf = buf1;
  blockRead.aio_nbytes = intSize*block_size*block_size;

  memset(&blockWrite, 0, sizeof(struct aiocb));
  blockWrite.aio_fildes = 1;
  blockWrite.aio_offset = 0;
  blockWrite.aio_buf = buf3;
  blockWrite.aio_nbytes = intSize*block_size*block_size;

  int scalar = (rand() % 201) - 100;
  fprintf(stderr,"scalar = %d\n", scalar);

  aio_read(&blockRead);
  while(aio_error(&blockRead) == EINPROGRESS)
  {
    ;
  }
  aio_return(&blockRead);

  for(int current = 0; current < (blocks*blocks)-1; current++)
  {
      int last = current - 1;
      int next = current + 1;
      memcpy(buf2, buf1, intSize*block_size*block_size);
      blockRead.aio_offset = intSize*block_size*block_size*next;
      aio_read(&blockRead);
      matrix_add(buf2, block_size, scalar);
      if(last >= 0)
      {

        if(last != 0)
        {
          blockWrite.aio_offset += intSize*block_size*block_size;
        }

        aio_write(&blockWrite);
        while(aio_error(&blockWrite) == EINPROGRESS)
        {
          ;
        }
        aio_return(&blockWrite);

      }

      memcpy(buf3, buf2, intSize*block_size*block_size);

      aio_return(&blockRead);

      memcpy(buf2, buf1, intSize*block_size*block_size);

  }

  matrix_add(buf2, block_size, scalar);
  blockWrite.aio_offset += intSize*block_size*block_size;
  aio_write(&blockWrite);
  while(aio_error(&blockWrite) == EINPROGRESS)
  {
    ;
  }
  aio_return(&blockWrite);

  memcpy(buf3, buf2, intSize*block_size*block_size);
  blockWrite.aio_offset += intSize*block_size*block_size;
  aio_write(&blockWrite);
  while(aio_error(&blockWrite) == EINPROGRESS)
  {
    ;
  }
  aio_return(&blockWrite);


  time_t end_time = time(NULL);
  time_t timeEllapsed = end_time - start_time;

  fprintf(stderr, "Overall time = %ld\n", timeEllapsed);

}
