#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>

const int long_size   = sizeof(long);
const int BUFFER_SIZE = 100; 
const int TIME_BUFFER = 30;

void  getdata(pid_t child, long addr,char *str, int len);

char* gettime();

void error_exit(char* msg);

int main(int argc, char *argv[])
{   
    pid_t traced_process;
    struct user_regs_struct regs;

    int status,
	sc;

    long addr;
    char child_cmdline[BUFFER_SIZE];
    char final_line[BUFFER_SIZE+TIME_BUFFER];
    char* time;

    if(argc != 2)
    {
        printf("Usage: %s <pid to be traced> <log file>\n",argv[0]);
        exit(1);
    }

    traced_process = atoi(argv[1]);

    /* Attaching to 'traced_process'. */
    if(ptrace(PTRACE_ATTACH, traced_process,NULL, NULL) == -1)
		error_exit("PTRACE_ATTACH failed.");

    /* Waiting for signal from 'traced_process'. */
    wait(&status);


    while(1)
    {
	/* Restart 'traced_process' and arrange it to be stopped at the next ENTRY-to
         * or EXIT-from a system call.
         */
	if(ptrace(PTRACE_SYSCALL,traced_process,NULL,NULL) == -1)
		error_exit("PTRACE_SYSCALL failed.");

	/* Wait for signal from 'traced_process'.*/
	wait(&status);

	if(WIFEXITED(status))
		break;
	else if(WIFSTOPPED(status))
	{
		/* Extracting registers contents to 'regs' struct after receiving signal from 'traced_process'. */
		if(ptrace(PTRACE_GETREGS,traced_process,NULL,&regs) == -1)
			error_exit("PTRACE_GETREGS failed.");

		/* Getting the number of the system call stored in orig_eax. */
		sc = regs.orig_eax;

		if(sc == SYS_read )
		{
			/* ecx register holds the wanted data so we will take its address. */
			addr = regs.ecx;
		
			/* Copy 'addr' contents into 'child_cmdline' from process 'traced_process'. */
			getdata(traced_process,addr,child_cmdline,BUFFER_SIZE);

			time = gettime();	        
			/* Printing command typed in 'traced_process' ex: (00:00:00) -> ls -l */
			sprintf(final_line,"(%s) -> %s",time,child_cmdline);
			printf("%s",final_line);
	
			free(time);
		 }
	}
     }

    /* Detaching from process 'traced_process'. */
    ptrace(PTRACE_DETACH, traced_process,NULL, NULL);

    return EXIT_SUCCESS;
}


void getdata(pid_t child, long addr,char *str, int len)
{   
    char *laddr;
    int i, j;

    union u
    {
            long val;
            char chars[long_size];
    }data;

    i = 0;
    j = len / long_size;
    laddr = str;

    while(i < j)
    {
        if((data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * 4, NULL)) == -1)
		error_exit("PTRACE_PEEKDATA failed.");
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }

    j = len % long_size;

    if(j != 0)
    {
        if((data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * 4, NULL)) == -1)
		error_exit("PTRACE_PEEKDATA failed.");
        memcpy(laddr, data.chars, j);
    }

    str[len] = '\0';
}

char* gettime()
{
        char* buff = malloc( TIME_BUFFER + 1 );

        if(buff == NULL)
		perror("malloc failed.");

        buff[TIME_BUFFER]  ='\0';
	struct timeval tv;

	time_t curtime;

        gettimeofday(&tv, NULL); 
        curtime=tv.tv_sec;

        strftime(buff,TIME_BUFFER,"%T",localtime(&curtime));

	return buff;
        
}

void  error_exit(char* msg)
{
	perror(msg);
	exit(1);
}
