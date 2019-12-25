#include <stdio.h>
#include "filesys.h"


unsigned int read(int fd, char *buf, unsigned int size)
{
	unsigned long off;
	int block, block_off;
	unsigned int i, j;
	struct inode *inode;
	char *temp_buf;

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD))
	{
		printf("\nthe file is not opened for read\n");
		return 0;
	}

	temp_buf = buf;

	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;

	if ((off + size) > inode->di_size)
		size = inode->di_size - off;

	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;						//{}block_off = off / BLOCKSIZ;

	if (block_off + size - 1  <= BLOCKSIZ)
	{
		/*
		fseek(fd, DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		*/
		memcpy(buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ + block_off, size);
		return size;
	}

	/*
	fseek(fd, DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ-block_off, fd);
	*/
	memcpy(temp_buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ + block_off, 
	 	      BLOCKSIZ-block_off);

	temp_buf += BLOCKSIZ - block_off;
	j = block + 1;																//}}j = (inode->di_size-off-block_off) / BLOCKSIZ;
	for (i = 0; i < (size - (BLOCKSIZ - block_off)) / BLOCKSIZ; i++)			//{}for (i = 0; i < (size - block_off) / BLOCKSIZ; i++)
	{
		/*
		fseek(fd, DATASTART+inode->di_addr[j+i]*BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		*/
		memcpy(temp_buf, disk+DATASTART+inode->di_addr[j+i]*BLOCKSIZ, BLOCKSIZ);
		temp_buf += BLOCKSIZ;
	}

	if((size - (BLOCKSIZ - block_off)) % BLOCKSIZ != 0)
	{
		//block_off = (size - block_off) % BLOCKSIZ;
		//block = inode->di_addr[off+size/BLOCKSIZ+1];
	/*
	fseek(fd, DATASTART+block*BLOCKSIZ, SEEK_SET);
	fread(temp_buf, 1, block_off, fd);
	*/
		memcpy(temp_buf, disk+DATASTART+inode->di_addr[j+i]*BLOCKSIZ, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ);
	}

	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;

	return size;
	
}


unsigned int write(int fd, char *buf, unsigned int size)
{
	unsigned long off;
	int block, block_off;
	unsigned int i, j, k;
	struct inode *inode;
	char *temp_buf;

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;

	if(!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE) && !(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FAPPEND))
	{
		printf("\nthe file is not opened for write\n");
		return 0;
	}

	temp_buf = buf;
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;
	//分配物理块
	if(inode->di_size < off + size)
	{
		i = inode ->di_size / BLOCKSIZ;
		if(inode ->di_size % BLOCKSIZ > 0)
			i ++;
		
		j = (off + size - 1) / BLOCKSIZ;
		if(j > 9)
		{
			printf("\nwriting data so long\n");
			return 0;
		}
		if((off + size - 1) % BLOCKSIZ > 0)
			j ++;
		
		k = i;
		for(; i < j; i ++)
		{
			block = balloc();
			if(block == DISKFULL)
			{
				//刚分配的物理块收回掉
				for(; k < i; k ++)
					bfree(inode ->di_addr[k]);
				return 0;
			}
			inode ->di_addr[i] = block;
		}
	}
	inode ->di_size = (unsigned short)off + size;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;									

	if(block_off + size - 1 <= BLOCKSIZ)						//若不超过一个盘块，即要写的内容能存放在一个物理块
	{
		memcpy(disk + DATASTART + inode -> di_addr[block] * BLOCKSIZ + block_off, buf, size);
	}
	else
	{
		memcpy(disk + DATASTART+inode -> di_addr[block] * BLOCKSIZ + block_off, temp_buf,
					 BLOCKSIZ - block_off);

		temp_buf += BLOCKSIZ - block_off;
		for (i = 0; i < (size - (BLOCKSIZ- block_off)) / BLOCKSIZ; i++)				//{}for (i = 0; i < (size - block_off) / BLOCKSIZ; i++)
		{
			memcpy(disk + DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, temp_buf, BLOCKSIZ);
			temp_buf += BLOCKSIZ;
		}
		
		if((size - (BLOCKSIZ - block_off)) % BLOCKSIZ != 0)
		{
			memcpy(disk + DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, temp_buf, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ);
		}
	}
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;
	return size;
}


