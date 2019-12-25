#include <stdio.h>
#include "filesys.h"

static struct dinode block_buf[BLOCKSIZ/DINODESIZ];

struct inode * ialloc()
{
	struct inode *temp_inode;
	int j;
	unsigned int cur_di = filsys.s_rinode;
	unsigned int tmp; 	
	unsigned i = 0, count = 0;								//count:���ҵĿ���i�ڵ����
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
			while(count < NICINOD && count < filsys.s_ninode && cur_di < tmp)		//ѭ������
			{
				if(block_end_flag)
				{
					memcpy(block_buf, disk + DINODESTART + cur_di * DINODESIZ, BLOCKSIZ);		//�����µ�һ�������		
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
			
				if(i >= BLOCKSIZ/DINODESIZ)												//�������ҳ����block_end_flag��Ϊ�㣬�Ա�����һ��ѭ���ж����µ������
				{
					block_end_flag = 0;
				}
			}

			if(j == 0)
			{
				if(cur_di >= MAX_DINODEID)													//�������һ��i�ڵ�
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
		temp_inode ->di_mode = DEFAULTMODE;						//}��ʶ��i�ڵ��ѷ���
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















































