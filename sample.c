/**
 * Autor: faguiar@parks
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

static int A[2048];
static int B[2048];
static int C[2048];
static int D[2048];
static int E[2048];
static int F[2048];
static int G[2048];
static int H[2048];
static int I[2048];

int main(int argc, char *argv[])
{
  int i;

  printf( "\n\n*** HELLO WORLD TEST ***\n" );
  printf( "Hello World\n" );
  printf( "*** END OF HELLO WORLD TEST ***\n" );

  for (i = 0; i <= 2047; i++)
  {
       A[i] = i;
       B[i] = i;
       C[i] = i;
       D[i] = i;
       E[i] = i;
       F[i] = i;
       G[i] = i;
       H[i] = i;
       I[i] = i;
  }

  printf("Addr of A %x\n",A);
  printf("Addr of A[0] %x\n", A[0]);
  while(1)
  {
      for (i = 0; i <= 2047; i++)
      {
        if (A[2] != 2)
        {
           perror("Memory Fault");
           exit(2);
        }
 

     }
   }
    
	return 0;
}

