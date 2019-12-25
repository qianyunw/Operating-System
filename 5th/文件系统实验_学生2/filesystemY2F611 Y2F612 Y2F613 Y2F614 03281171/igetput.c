#include <stdio.h>
#include "filesys.h"

struct inode * iget(unsigned int dinodeid)
{
	int existed = 0; 
	int	inodeid;
	long addr;
	struct inode *temp, *newinode;

	inodeid = dinodeid % NHINO;
	if (hinode[inodeid].i_forw == NULL)
		existed = 0;
	else
	{
		temp = hinode[inodeid].i_forw; 
		while (temp)
		{
			if (temp->i_ino == dinodeid)  //xiao
			/* existed */
			{
				existed = 1;
				temp->i_count ++;
				return temp;
			}
			else  /*not existed*/
				temp = temp->i_forw;

		}
	}

	/* 如该磁盘节点没被读入
	/* not existed */   
	/* 1. calculate the addr of dinode in the file sys column*/
	addr = DINODESTART + dinodeid * DINODESIZ;

	/* 2. malloc the new inode*/
	newinode = (struct inode *)malloc(sizeof(struct inode));

	/* 3. read the dinode to the inode*/
	/*
	fseek(fd, addr, SEEK_SET);
	fread(&(newinode->di_number), DINODESIZ, 1, fd);
	*/
	memcpy(&(newinode->di_number), disk + addr, DINODESIZ);

	/* 4. put it into hinode[inodeid] queue*/
	newinode->i_forw = hinode[inodeid].i_forw;		/*把该节点插入到最前边 */
	newinode->i_back = newinode;					
	if (newinode->i_forw)
		newinode->i_forw->i_back = newinode;
	hinode[inodeid].i_forw = newinode; 

	/*5. initialize the inode*/
	newinode->i_count = 1;
	newinode->i_flag = 0;   /* flag for not update*/
	newinode->i_ino = dinodeid;

	return newinode;
}


iput(struct inode *pinode)
{
	long addr;
	unsigned int block_num;
	int i;

	if (pinode->i_count > 1)
	{
		pinode->i_count --;
		return 0;
	}
	else
	{
		if (pinode->di_number != 0)			//若关联文件数不为零，则只把该内存i节点写回磁盘
		{
			/*write back the inode*/
			addr = DINODESTART + pinode->i_ino * DINODESIZ;
			/*
			fseek(fd, addr, SEEK_SET);
			fwrite(&pinode->di_number, DINODESIZ, 1, fd);
			*/
			memcpy(disk + addr, &pinode->di_number, DINODESIZ);
			if(pinode == cur_path_inode)
			{
				for(i = 0; i < dir.size / (BLOCKSIZ/(DIRSIZ + 4)); i ++)
				{
					memcpy(disk + DATASTART+ pinode->di_addr[i]*BLOCKSIZ, &dir.direct[BLOCKSIZ / (DIRSIZ + 4) * i], BLOCKSIZ);
				}
				if(dir.size % (BLOCKSIZ/(DIRSIZ + 4)) != 0)
					memcpy(disk + DATASTART+ pinode->di_addr[i]*BLOCKSIZ, &dir.direct[BLOCKSIZ / (DIRSIZ + 4) * i], dir.size * (DIRSIZ + 4) - BLOCKSIZ * i);
			}	

		}
		else								//若关联文件数为零，则把对应的数据区释放掉
		{
			/* rm the inode & the block of the file in the disk */
			block_num = pinode->di_size/BLOCKSIZ;	//得到数据区所占的物理块数
			for (i=0; i < block_num; i++)
				bfree(pinode->di_addr[i]);			//{}balloc(pinode->di_addr[i]);
			ifree(pinode->i_ino);
		}

		/*free the inode in the memory*/
		/*
		if (pinode->i_forw == NULL)
			pinode->i_back->i_forw = NULL;
		else
		{
			pinode->i_forw->i_back = pinode->i_back;
			pinode->i_back->i_forw = pinode->i_forw;
		}*/

		//added by xiao 
		{
			int inodeid;
			inodeid = (pinode->i_ino)  % NHINO;

			if (hinode[inodeid].i_forw == pinode)
			{
				hinode[inodeid].i_forw = pinode->i_forw;
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_forw;
			}
			else
			{
				pinode->i_back->i_forw = pinode->i_forw;
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_back;
			}
		}
		//end by xiao
		
		free(pinode);
			
	}
	return 0;
}