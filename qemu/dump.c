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

  int n = 0;
  int i = 0;
  pid_t pid;
  long word;

  if(argc != 4){
          printf("Usage:Please enter the value like ./dump &pid &base address [HEX] &offset [HEX]\n");
          exit(0);
  }
  pid=atoi(argv[1]);
  unsigned long BASE= strtoul(argv[2], NULL, 16);
  unsigned long ADDR= strtoul(argv[3], NULL, 16);
  unsigned long VIRT= BASE+ADDR;
  printf ("The input offset is %x, the base address is: %x,  its virtual address is %x\n", ADDR, BASE, VIRT);

  long result = ptrace(PTRACE_ATTACH,pid,NULL,NULL); 

  waitpid(pid,NULL,WUNTRACED);
  
  result = ptrace(PTRACE_PEEKDATA,pid,(void *)VIRT,&word);
  printf("The input address is: %x, its result is %x\n", VIRT, result);

  ptrace(PTRACE_DETACH,pid,NULL,NULL);
  exit(0);
}

