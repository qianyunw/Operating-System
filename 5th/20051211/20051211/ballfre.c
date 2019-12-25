#include <stdio.h>
#include "filesys.h"

static unsigned int block_buf[BLOCKSIZ];

unsigned int balloc()
{
	unsigned int free_block, free_block_num;
	int i;

	if (filsys.s_nfree == 0)
	{
		printf("\nDisk Full!!!\n");
		return DISKFULL;
	}

	free_block = filsys.s_free[filsys.s_pfree]; 
	if (filsys.s_pfree == NICFREE-1)
	{ 
		/*
		fseek(fd, DATASTART+filsys.s_free[filsys.s_pfree]*BLOCKSIZ, SEEK_SET);
		fread(block_buf, 1, BLOCKSIZ, fd);
		*/
		memcpy(block_buf, disk+DATASTART+filsys.s_free[filsys.s_pfree]*BLOCKSIZ, BLOCKSIZ);
		free_block_num = block_buf[NICFREE];  /*the local block num in the group*/
		/*
		for (i=0; i<free_block_num; i++)
			filsys.s_free[NICFREE-1-i] = block_buf[i];
		filsys.s_pfree = NICFREE - free_block_num;
		*/
		for (i=0; i<NICFREE; i++)
			filsys.s_free[i] = block_buf[i];
		filsys.s_pfree = 0;
	}  
	else 
		filsys.s_pfree++;

	filsys.s_nfree --;

	filsys.s_fmod = SUPDATE;

	return free_block;

	
}


bfree(block_num)
unsigned int block_num;
{
	int i;

	if (filsys.s_pfree == 0)   /* s_free full*/
	{
		block_buf[NICFREE]=NICFREE;
		for(i=0; i<NICFREE; i++)
			block_buf[i] = filsys.s_free[NICFREE-1-i];
		filsys.s_pfree = NICFREE-1;
	}

	
	fseek(fd, DATASTART+block_num*BLOCKSIZ, SEEK_SET);//add by xiao
	fwrite(block_buf,1,BLOCKSIZ,fd);
	
	filsys.s_nfree++;
	filsys.s_fmod = SUPDATE;
}








































