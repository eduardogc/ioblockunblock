#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/string.h>

char ioBlock[128][256];
EXPORT_SYMBOL(ioBlock);

asmlinkage int sys_io_block(char *part)
{
  int i;
	if(ioBlock[0][0] == 127)
	    return -1; // Tabela cheia
	else
	{
	  for (i = 1; i <= ioBlock[0][0]; ++i)
		if(strcmp(ioBlock[i],part) == 0)
			return 1; // Se particao ja estiver na tabela
	}
		
	strcpy(ioBlock[ioBlock[0][0] + 1], part);
	++ioBlock[0][0];
	    
    return 0; // Se for incluido com sucesso
}
