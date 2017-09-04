#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main ()
{
     int errno;
     long len = 8;
     const int buflen = (len / sizeof(long)) + 1;
     void *buffer;
     long *base;
     char *data;
     int i;
     //the process pid i want to track
     pid_t pid = 25515;
     //the address that the string we want to read resides 
     void *addr = (unsigned long int*) 0x00601080;

     buffer = calloc(buflen, sizeof(long));
     if (NULL == buffer)
     {
         printf("Fault at allocation");    
     }

     base = (long *) buffer; 

     ptrace(PTRACE_ATTACH, pid, NULL, NULL);

     for (i = 0; i < buflen; i++) {
            if(ptrace(PTRACE_PEEKDATA, pid , addr + (sizeof(long) * i),NULL) !=  -1)
            {
                    *(base + i) = ptrace(PTRACE_PEEKDATA, pid , addr + (sizeof(long) * i), NULL);
            }
            else
            {
                    fprintf(stderr, "Value of errno: %s\n", strerror(errno));
            }
      }

     ptrace(PTRACE_DETACH, pid, NULL, NULL);

     /* Pop a null at the end, since we're printing this string. */
     *((char *) buffer + len) = '\0';

     data = (char *)buffer;  

     printf("%p[\"%s\"]\n",addr, data);
     free(buffer);
     }
  
