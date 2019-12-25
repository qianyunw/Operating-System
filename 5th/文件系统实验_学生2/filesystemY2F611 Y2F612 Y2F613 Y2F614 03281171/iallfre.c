#include <stdio.h>
#include "filesys.h"

static struct dinode block_buf[BLOCKSIZ/DINODESIZ];

struct inode * ialloc()
{
	struct inode *temp_inode;
	int j;
	unsigned int cur_di = filsys.s_rinode;
	unsigned int tmp; 	
	unsigned i = 0, count = 0;								//count:查找的空闲i节点个数
	int block_end_flag = 1;
	if (filsys.s_pinode == NICINOD)   /* s_inode empty*/
	{
		for(j = 0; i < 2; j ++)
		{
			if(j == 0)
				tmp = MAX_DINODEID;
			else
				tmp = filsys.s_rinode;
			block_end_flag = 1;
			while(count < NICINOD && count < filsys.s_ninode && cur_di < tmp)		//循环查找
			{
				if(block_end_flag)
				{
					memcpy(block_buf, disk + DINODESTART + cur_di * DINODESIZ, BLOCKSIZ);		//读入新的一个物理块		
					block_end_flag = 0;
					i = 0;
				}
				
				if(block_buf[i].di_mode == DIEMPTY)
				{
					filsys.s_pinode --;
					filsys.s_inode[filsys.s_pinode] = cur_di;
					count ++;
				}
				cur_di ++;
				i ++;
			
				if(i >= BLOCKSIZ/DINODESIZ)												//若读完该页，则block_end_flag设为零，以便在下一个循环中读入新的物理块
				{
					block_end_flag = 0;
				}
			}

			if(j == 0)
			{
				if(cur_di >= MAX_DINODEID)													//若到最后一个i节点
				{
					cur_di = 0;
					continue;
				}
				else
				{
					break;
				}
			}
		}
		filsys.s_rinode = cur_di;
	}

	if(filsys.s_pinode != NICINOD)
	{
		temp_inode = iget(filsys.s_inode[filsys.s_pinode]);
		temp_inode ->di_mode = DEFAULTMODE;						//}标识该i节点已分配
		/*
		fseek(fd, DINODESTART+filsys.s_inode[filsys.s_pinode]*DINODESIZ, SEEK_SET);
		fwrite(&temp_inode->di_number, 1, sizeof(struct dinode), fd);
		*/
		memcpy(disk + DINODESTART + filsys.s_inode[filsys.s_pinode] * DINODESIZ,
					 &temp_inode->di_number, sizeof(struct dinode));
		filsys.s_pinode ++;
		filsys.s_ninode --; 
		filsys.s_fmod = SUPDATE; 
		return temp_inode;
	}
	else
	{
		printf("did not find out any empty i node!\n");
		return NULL;
	}
} 


ifree(unsigned int dinodeid)
{
	//added by cholguk
	struct dinode temp_dinode;						//}	
	temp_dinode.di_mode = DIEMPTY;					//}
	memcpy(disk + DINODESTART + dinodeid * DINODESIZ, &temp_dinode, DINODESIZ);
	//
	
	filsys.s_ninode ++;
	if(filsys.s_pinode > 0)        /*not null*/ //{}if (filsys.s_pinode != NICINOD)
	{
		filsys.s_pinode --;					//{}filsys.s_pinode++;
		filsys.s_inode[filsys.s_pinode] = dinodeid;
	}
	else   /*full*/
	{
		if (dinodeid < filsys.s_rinode)
		{
												//{}filsys.s_inode[NICINOD] = dinodeid;
												//}}filsys.s_rinode = dinodeid;
		}
	}
	return 0;
}















































