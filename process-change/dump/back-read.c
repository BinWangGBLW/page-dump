#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
//#include <libexplain/ptrace.h>

/* testread.c */

//#define ADDR 0x60108C

int main(int argc,char *argv[])
{

  pid_t pid;
  long word;

  unsigned long addr= strtoul(argv[2], NULL, 16);

  if(argc != 3){
          printf("Usage:testread pid\n");
          exit(0);
  }
  pid=atoi(argv[1]);
  unsigned long ADDR= strtoul(argv[2], NULL, 16);
  long result = ptrace(PTRACE_ATTACH,pid,NULL,NULL); 

  waitpid(pid,NULL,WUNTRACED);

  result = ptrace(PTRACE_PEEKDATA,pid,(void *)ADDR,&word);
  printf("The address is :%x, the result is %x\n", ADDR, result);


  ptrace(PTRACE_DETACH,pid,NULL,NULL);
  exit(0);
}

