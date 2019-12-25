#include <stdio.h>
#include "filesys.h"


unsigned short creat(unsigned int user_id, char *filename, unsigned short mode)
{
	unsigned int di_ith, di_ino;
	struct inode *inode;
	int i,j;
	int user_sen;

	//added by xiao user_id-->user_sen
	for (j=0; j<USERNUM; j++)   //xiao
		if (user[j].u_uid == user_id) 
			break;
	if (j == USERNUM)
	{
		printf("\n have no correspond user_id.\n ");
		return NULL;
	}
	user_sen = j;
	//end by xiao

	di_ino = namei(filename);
	if (di_ino != NULL)    /*already existed*/
	{
		inode = iget(di_ino);
		if (access(user_sen, inode, mode) == 0)
		{
			iput(inode);
			printf("\n creat access not allowed \n");
			return NULL;
		}

		/* free all the block of the old file */
		for (i=0; i<inode->di_size/BLOCKSIZ+1; i++)
			bfree(inode->di_addr[i]);

		/* to do: add code here to update the pointer of the sys_file*/
		for (i=0; i<SYSOPENFILE; i++)
			if (sys_ofile[i].f_inode == inode)
				sys_ofile[i].f_off = 0;

		for (i=0; i<NOFILE; i++)
			if (user[user_sen].u_ofile[i] == SYSOPENFILE+1)
			{
				user[user_sen].u_uid = inode->di_uid;
				user[user_sen].u_gid = inode->di_gid;

				for (j=0; j<SYSOPENFILE; j++)
					if (sys_ofile[j].f_count == 0)   //xiao
					{
						user[user_sen].u_ofile[i] = j;
						sys_ofile[j].f_flag = mode;
					}

				return;
			}
	}
	else   /*not existed before*/
	{
		inode = ialloc();
		di_ith = iname(filename);

		dir.size++; 
 
		dir.direct[di_ith].d_ino = inode->i_ino;
		//inode->di_mode = user[user_sen].u_default_mode;
		inode->di_mode = mode;
		inode->di_uid = user[user_sen].u_uid;
		inode->di_gid = user[user_sen].u_gid; 
		inode->di_size = 0;
		inode->di_number = 0; 

		for (i=0; i<SYSOPENFILE; i++)
			if (sys_ofile[i].f_count == 0)
				break; 

		for (j=0; j<NOFILE; j++)   //xiao
			if (user[user_sen].u_ofile[j] == SYSOPENFILE+1)
				break;

		user[user_sen].u_ofile[j] = i;
		sys_ofile[i].f_flag = mode;
		sys_ofile[i].f_count = 1;  //xiao 0-->1 
		sys_ofile[i].f_off = 0;
		sys_ofile[i].f_inode = inode;

		return j;
	}
}







































