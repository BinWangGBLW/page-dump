/*
 * B.Wang: dump and bit-flip the content of physical address
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#include "debug.h"

#include "devmem.h"

/*
 * define the debug level of this file,
 * please see 'debug.h' for detail info
 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_INFO);

/* max buffer size in byte */
#define MAX_BUF_SIZE		4096

int main(int argc, char **argv)
{
	int width = 32, count = 1, i;
	unsigned long addr, dvalue;
	unsigned char buf[MAX_BUF_SIZE];

	if (argc < 2) {
		ERR("\nUsage:\t%s { address } [type] [count]\n"
			"\taddress:	Address to flip\n"
			"\twidth:	Width (8/16/...), default is 32\n"
			"\tcount	Data count to be read, default is 1\n\n",
			argv[0]);
		return -1;
	}
	addr = strtoul(argv[1], 0, 0);

	if (argc > 2) {
		width = strtoul(argv[2], 0, 0);
	}

	if (argc > 3) {
		count = strtoul(argv[3], 0, 0);
	}

	if (width * count > MAX_BUF_SIZE) {
		ERR("width * count(%d) should little than %d\n",
			width * count, MAX_BUF_SIZE);
		return -1;
	}

	DEBUG("addr 0x%lx, width %c, count %d\n", addr, width, count);

	switch (width) {
	case 8:
		devmem_readsb(addr, (void *)buf, count);

		for (i = 0; i < count; i++)
			INFO("The old value of 0x%08lx is: 0x%02x\n", addr + i, *(buf + i));
		
		sscanf(buf,"%x",&dvalue);
		dvalue= devmem_flip(dvalue);

		*buf=dvalue;
			
		/*Wrote the new value to the same physical address*/ 
		devmem_writeb(addr, (void *)buf, count);

		/*This is to show the new value of this address*/

		for (i = 0; i < count; i++)
		   INFO("The new value of 0x%08lx is: 0x%02x\n", addr + i, *(buf + i));
		break;

	case 16:
		devmem_readsw(addr, (void *)buf, count);

		for (i = 0; i < count; i++)
			INFO("The old value of 0x%08lx is: 0x%04x\n", addr + i * 2,
				*((unsigned short *)(buf + i * 2)));
		break;

	case 32:
		devmem_readsl(addr, (void *)buf, count);

		for (i = 0; i < count; i++)
			INFO("The old value of 0x%08lx is: 0x%08x\n", addr + i * 4,
				*((unsigned int *)(buf + i * 4)));
		break;

	case 64:
		devmem_readsq(addr, (void *)buf, count);

		for (i = 0; i < count; i++)
			INFO("The old value of 0x%08lx is: 0x%08lx\n", addr + i * 8,
				*((unsigned long *)(buf + i * 8)));
		break;

	default:
		ERR("width(%d) invalid\n", width);
		return -1;
	}


	return 0;
}

