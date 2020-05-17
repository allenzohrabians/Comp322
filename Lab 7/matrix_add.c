#include <stdio.h>    // printf, stderr, fprintf
#include <string.h>   // memcpy, memset
#include <stdlib.h>   // atoi, rand, srand
#include <time.h>     // time
#include <aio.h>      // aiocb, aio_fildes, aio_return, aio_nbytes, aio_error, aio_write, aio_read, aio_buf, aio_offset
#include <errno.h>    // errno - number of last error

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

    x = atoi(newBuf);   // atoi - converts the string argument to an intege

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

  srand(time(NULL));    // Makes use of the computer's internal clock to control the choice of the seed

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

  memset(&blockRead, 0, sizeof(struct aiocb));  // memset - fills the first n bytes of the memory areapointed to by blockRead with the constant byte 0.

  blockRead.aio_fildes = 0;
  blockRead.aio_offset = 0;
  blockRead.aio_buf = buf1;
  blockRead.aio_nbytes = intSize*block_size*block_size;

  memset(&blockWrite, 0, sizeof(struct aiocb));

  blockWrite.aio_fildes = 1;                                // aio_fildes - The file descriptor on which the I/O operation is to be performed
  blockWrite.aio_offset = 0;                                // aio_offset - This is the file offset at which the I/O operation is to be performed
  blockWrite.aio_buf = buf3;                                // aio_buf - This is the buffer used to transfer data for a read or write operation
  blockWrite.aio_nbytes = intSize*block_size*block_size;    // aio_nbytes - This is the size of the buffer pointed to by aio_buf

  int scalar = (rand() % 201) - 100;
  fprintf(stderr,"scalar = %d\n", scalar);    // Random scalar value

  aio_read(&blockRead);   // aio_read - queues the I/O request described by the buffer pointed to by blockRead

  while(aio_error(&blockRead) == EINPROGRESS)   // aio_error - returns the error status for the asynchronous I/O request with control block pointed to by aiocbp
  {                                             // EINPROGRESS - if the request has not been completed yet
    ;
  }

  aio_return(&blockRead);   // aio_return - returns the final return status for the asynchronous I/O request with control block pointed to by blockRead

  for(int current = 0; current < (blocks*blocks)-1; current++)
  {
      int last = current - 1;
      int next = current + 1;

      memcpy(buf2, buf1, intSize*block_size*block_size);    // memcpy - copies (intSize*block_size*block_size) bytes from buf1 to buf2

      blockRead.aio_offset = intSize*block_size*block_size*next;

      aio_read(&blockRead);

      matrix_add(buf2, block_size, scalar);

      if(last >= 0)
      {

        if(last != 0)
        {
          blockWrite.aio_offset += intSize*block_size*block_size;
        }

        aio_write(&blockWrite);   // aio_write - queues the I/O request described by the buffer pointed to by aiocbp

        while(aio_error(&blockWrite) == EINPROGRESS)
        {
          ;
        }
        aio_return(&blockWrite);

      }

      memcpy(buf3, buf2, intSize*block_size*block_size);    // memcpy - copies (intSize*block_size*block_size) bytes from buf2 to buf3

      aio_return(&blockRead);

      memcpy(buf2, buf1, intSize*block_size*block_size);    // memcpy - copies (intSize*block_size*block_size) bytes from buf1 to buf2

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
