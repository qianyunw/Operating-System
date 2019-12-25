#include <stdio.h>
#include "filesys.h"

creat(unsigned short uid, char *filename, unsigned short mode)
{
	unsigned int dirpos, di_ino;
	struct inode *inode;
	int i,j;
	int user_id;
	user_id = uid_to_userid(uid);
	if(user_id >= USERNUM)
	{
		return -1;
	}

	di_ino = namei(filename);
	if (di_ino != 0)    /*already existed*/
	{
		inode = iget(di_ino);
		if (access(user_id, inode, mode) == 0)
		{
			iput(inode);
			printf("\n creat access not allowed \n");
			return -1;
		}

		/* free all the block of the old file */
		for (i = 0; i < inode->di_size/BLOCKSIZ+1; i++)
			bfree(inode->di_addr[i]);

		/* to do: add code here to update the pointer of the sys_file*/
		for (i = 0; i < SYSOPENFILE; i ++)
			if (sys_ofile[i].f_inode == inode)
				sys_ofile[i].f_off = 0;

		for (i = 0; i < NOFILE; i ++)
			if (user[user_id].u_ofile[i] == SYSOPENFILE+1)
			{
				user[user_id].u_uid = inode->di_uid;
				user[user_id].u_gid = inode->di_gid;

				for (j = 0; j < SYSOPENFILE; j ++)
					if (sys_ofile[j].f_count == 0)   //xiao
					{
						user[user_id].u_ofile[i] = j;
						sys_ofile[j].f_flag = mode;
					}

				return -1; 
			}
	}
	else   /*not existed before*/
	{
		inode = ialloc();
		dirpos = iname(filename);
		if(dirpos == 0)
		{	
			iput(inode);			//}
			return -1;
		}
		dir.size++; 
		cur_path_inode->di_size += DIRSIZ + 4;

		dir.direct[dirpos].d_ino = inode->i_ino;
		//inode->di_mode = user[user_id].u_default_mode;
		inode->di_number = 1;					//{}inode->di_number = 0;
		inode->di_mode = mode;
		inode->di_uid = user[user_id].u_uid;
		inode->di_gid = user[user_id].u_gid; 
		inode->di_size = 0;
														

		for (i = 0; i < SYSOPENFILE; i++)
			if (sys_ofile[i].f_count == 0)
				break;

		for (j=0; j<NOFILE; j++)   //xiao
			if (user[user_id].u_ofile[j] == SYSOPENFILE+1)
				break;

		user[user_id].u_ofile[j] = i;
		sys_ofile[i].f_flag = mode;
		sys_ofile[i].f_count = 1;			//{}sys_ofile[i].f_count = 0;
		sys_ofile[i].f_off = 0;
		sys_ofile[i].f_inode = inode;
	}
	return j;
}
































