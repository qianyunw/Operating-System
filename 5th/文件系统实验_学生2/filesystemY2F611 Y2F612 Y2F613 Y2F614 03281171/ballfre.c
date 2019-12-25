#include <stdio.h>
#include "filesys.h"

static unsigned int block_buf[BLOCKSIZ/sizeof(int)];		//{}static unsigned int block_buf[BLOCKSIZ];

unsigned int balloc()
{
	unsigned int free_block;
	int i;

	if (filsys.s_nfree == 0)
	{
		printf("\nDisk Full!!!\n");
		return DISKFULL;
	}

	free_block = filsys.s_free[filsys.s_pfree];		
	if(filsys.s_pfree == NICFREE - 1)
	{
		
		/* fread(block_buf, 1, BLOCKSIZ, fd);
		free_block_num = block_buf[NICFREE];  /*the local block num in the group
		for (i=0; i<free_block_num; i++)
			filsys.s_free[NICFREE-1-i] = block_buf[i];
		filsys.s_pfree = NICFREE - free_block_num;
		}
		*/
		memcpy(block_buf, disk + DATASTART + filsys.s_free[filsys.s_pfree] * BLOCKSIZ, BLOCKSIZ);
		for(i = 0; i < NICFREE - 1; i ++)
		{
			if(block_buf[i] > FILEBLK)
				break;
		}	
		for(; i >= 0; i --)
		{
			filsys.s_free[filsys.s_pfree --] = block_buf[i];
		}
	}
	
	filsys.s_pfree++;
	filsys.s_nfree --;
	filsys.s_fmod = SUPDATE;
	return free_block;
}


bfree(unsigned int block_num)
{
	int i;
	if(filsys.s_pfree == 0)		/*s_free full*/
	{
		/*把栈中内容取出来放到block_buf */
		for(i = 0; i < NICFREE; i ++)
		{
			block_buf[i] = filsys.s_free[i];
		}
		/*把栈中的内容，即block_buf的内容写到回收节点*/
		memcpy(disk + DATASTART + block_num * BLOCKSIZ, block_buf, BLOCKSIZ);
		/*把栈低的内容设为刚回收的节点*/
		filsys.s_pfree = NICFREE - 1;
		filsys.s_free[filsys.s_pfree] = block_num;
	}
	else
	{
		filsys.s_pfree --;
		filsys.s_free[filsys.s_pfree] = block_num;
	}
	filsys.s_nfree ++;
	filsys.s_fmod = SUPDATE;	
	/*
	int i;

	if (filsys.s_pfree == 0)   /* s_free full
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
	*/
	return 0;
}








































