#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>




int main(int argc, char* argv[]) {

  if (argc != 3) {
    fprintf(stderr, "usage: ERROR_RATE CMD ARGS...\n"); exit(1);
  }

  unsigned long pid = strtoul(argv[1], NULL, 10);
  unsigned long addr= strtoul(argv[2], NULL, 16);
 
  printf("pid is: %ld, address is: %lx\n", pid, addr);

  if (pid == 0) {
    fprintf(stderr, "exec() did not succeed\n"); exit(1);
  }
 
  else if (pid > 0) {
        char procmap_fn[128];
        sprintf(procmap_fn, "/proc/%d/maps", pid);
        ptrace(PTRACE_SEIZE, pid, 0, 0);

         printf("procmap_fn is :%s\n", procmap_fn);

       ptrace(PTRACE_INTERRUPT, pid, 0, 0);
      waitpid(pid, NULL, WSTOPPED);
      int n = 0;
      while (n <= 10 * sizeof(int))
      {
           uint32_t orig = ptrace(PTRACE_PEEKDATA, pid, (addr+n), NULL);
           //uint32_t orig = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
           printf("The address is: %lx\n, The data is: %lx\n", addr+n, orig);
           n += sizeof(int);
       }
      ptrace(PTRACE_CONT, pid, 0, 0);
  }
}
