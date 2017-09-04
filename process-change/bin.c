#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


//#error "This program damages things, and should be run on a virtual machine where that's okay"


int main() {
 

    char procmap_fn[128];
    sprintf(procmap_fn, "/proc/19298/maps");
    

    printf("procmap_fn is :%s\n", procmap_fn);

    int pid =19298;

    ptrace(PTRACE_SEIZE, pid, 0, 0);

    uint32_t addr;
    sscanf("602080", "%lx", &addr);
    uint32_t orig = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    printf("The address is: %lx\n, The data is: %lx\n", addr, orig);

    ptrace(PTRACE_CONT, pid, 0, 0);
}
