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

  unsigned long value, flip_mark;
  int flip_no;
 
  srand(time(0));

  unsigned long addr= strtoul(argv[2], NULL, 16);

  if(argc != 3){
          printf("Usage: Please enter the value like ./flip &pid &address\n");
          exit(0);
  }
  pid=atoi(argv[1]);
  unsigned long ADDR= strtoul(argv[2], NULL, 16);
  long result = ptrace(PTRACE_ATTACH,pid,NULL,NULL); 

  waitpid(pid,NULL,WUNTRACED);
  
  result = ptrace(PTRACE_PEEKDATA,pid,(void *)ADDR,NULL);
  printf("The address is of is: %x, its old result is %x\n", ADDR, result);
  
  flip_no = rand()%8;
  printf("The number of bit that is flip is %d\n", flip_no);
  flip_mark = 1 << flip_no;
  value = result^flip_mark;
  printf("The new result of address %x is %x\n", ADDR, value);

  ptrace(PTRACE_POKEDATA, pid, ADDR, value);
  ptrace(PTRACE_DETACH,pid,NULL,NULL);


  exit(0);
}

