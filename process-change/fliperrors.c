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


int main(int argc, char* argv[]) {
  srand(time(0));

  if (argc < 3) {
    fprintf(stderr, "usage: ERROR_RATE CMD ARGS...\n"); exit(1);
  }

  // error_rate -- approx. errors / kilobyte*microsecond
  float error_rate; sscanf(argv[1], "%f", &error_rate);
  const int error_cycle_interval = 100; // us
  float error_rate_per_cycle = error_rate * error_cycle_interval;

  int flip_count = 0;
  pid_t pid=fork();
  //int pid = strtol(argv[2],NULL,16);
  
  if (pid == 0) {
    execvp(argv[2], argv+2);
    fprintf(stderr, "exec() did not succeed\n"); exit(1);
  }
 
  else if (pid > 0) {
    ptrace(PTRACE_SEIZE, pid, 0, 0);

    // man proc(5)
    char procmap_fn[128];
    sprintf(procmap_fn, "/proc/%u/maps", pid);
    
    //int i;
    //for (i=0; i < 128; i++)
    //{
    //      printf("procmap_fn[%d] is:%s\n", i, procmap_fn[i]);
    //}

         printf("procmap_fn is :%s\n", procmap_fn);

    // while traced process still alive..
    while (0 == waitpid(pid, NULL, WNOHANG)) {

      ptrace(PTRACE_INTERRUPT, pid, 0, 0);
      waitpid(pid, NULL, WSTOPPED);

      FILE *procmap_file = fopen(procmap_fn, "r");
      if (procmap_file == NULL) break;

      // parse entries in /proc/[pid]/maps, description of memory map
      char map_descr[1024];
      //while (fgets((char*)map_descr, 1023, procmap_file)) {
      //	if (strstr(map_descr, "stack") || strstr(map_descr, "heap")) {
      //	  uint64_t low, high;
      //	  sscanf(map_descr, "%lx-%lx", &low, &high);

	//  int num_flips = (int)((high-low) * error_rate_per_cycle / 1024);
	  
	//  int j;
	  //for (j=0; j<num_flips; ++j) {
	  //  ++flip_count;
	    // random pointer & bit position to flip
	  //  uint32_t *addr = (uint32_t*)(low + (rand()%(high-low-4)));
	  //  uint32_t flip_mask = (1 << (rand()%32));
	  
	  //  uint32_t orig = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	   // ptrace(PTRACE_POKEDATA, pid, addr, (orig ^ flip_mask));
          // printf("The address is: %lx\n, The data is: %lx\n", &addr, orig);
	//  }
	//}
      //}
      uint32_t addr;
      sscanf("601018", "%lx", &addr);
      uint32_t orig = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
      printf("The address is: %lx\n, The data is: %lx\n", addr, orig);

      fclose(procmap_file);
      ptrace(PTRACE_CONT, pid, 0, 0);
      usleep(error_cycle_interval);
    }
  }
  printf("total bits flipped: %d\n", flip_count);
}
