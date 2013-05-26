#include <linux/kernel.h>
#include <linux/string.h>

asmlinkage int sys_io_unblock(char *part)
{
  extern char ioBlock[128][256];
	int i;
	int a = ioBlock[0][0];

	for (i = 1; i <= a; ++i)
	{ 
		if(strcmp(ioBlock[i], part) == 0)
		{
			strcpy(ioBlock[i], ioBlock[a]);
			a--; 
			ioBlock[0][0] = a;
			
			return 0; //returns 0 if success
		}		
	}
		
	return -1; //returns -1 if doesn't find element.
}
