#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>
#include <sys/mman.h>
#include <locale.h>

#define N 16 

int bit_flip(unsigned long value)
{
    int a[N];
    int i, flip, result;
    char buf[N];
    char *e;
    srand( (unsigned)time( NULL ) );
    
    for (i = 0; i != N; ++i)
    {
        a[N - 1 - i] = value % 2;
        value /= 2;
    }

    for (i= 0; i !=N; ++i)
      buf[i] = '0'+a[i];

    printf("The binary of old value is :%s\n", buf);
    result=strtol(buf,&e,2);
    printf("The hexadecimal of old value is : 0x%x\n", result);

    flip=rand()%N;
    printf("Notice!: The %d th bit will be flipped\n", flip);
    for (i = 0; i !=N; ++i)
        if(i==flip)
        {
            printf ("The old a[%d]'s value is: %d\n", i, a[i]);
            a[i]=a[i]^1;
            printf ("The new a[%d]'s value is: %d\n", i, a[i]);
        }

    for (i= 0; i !=N; ++i)
      buf[i] = '0'+a[i];

    printf("The binary of new value is :%s\n", buf);
    result=strtol(buf,&e,2);
    printf("The hexadecimal of new value is : 0x%x\n", result);
    return result;


}

int main(int argc, char *argv[]) 

{ 

    unsigned long n; 
    int result;
    
    if (argc > 2 || argc <2) {
		printf(" please enter a valided address\n");
		return 0;
	}

    sscanf(argv[1],"%x",&n);
    result = bit_flip (n);
    printf ("Result value is :%d\n", result);
    
    return 0;
    
}




