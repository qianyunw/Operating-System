#include <stdio.h>
#include "filesys.h"

unsigned short open(int user_id, char *filename, unsigned short openmode)
{
	unsigned int dinodeid;
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

	dinodeid = namei(filename);
	if (dinodeid == NULL)    /*  no such file */  //xiao
	{
		printf("\nfile does not existed!!!\n");
		return NULL;
	}
 
	inode = iget(dinodeid);
#if 0
	if (!access(user_id, inode, openmode))     /*access denied*/
	{
		printf("\nfile open has not access!!!\n");
		iput(inode);
		return NULL;
	}
#endif
	

	/*alloc the sys_ofile item*/
	for (i=0; i<SYSOPENFILE; i++)   //xiao  i=1-->i=0
		if (sys_ofile[i].f_count == 0)
			break;

	if (i == SYSOPENFILE)
	{
		printf("\nsystem open file too much\n");
		iput(inode);
		return NULL;
	}

	sys_ofile[i].f_inode = inode;
	sys_ofile[i].f_flag = openmode;
	sys_ofile[i].f_count = 1;

	if (openmode & FAPPEND)
		sys_ofile[i].f_off = inode->di_size;
	else
		sys_ofile[i].f_off = 0;

	/*alloc the user open file item*/
	for (j=0; j<NOFILE; j++)
		if (user[user_sen].u_ofile[j] == SYSOPENFILE+1)  //xiao
			break;

	if (j == NOFILE)  
	{
		printf("\nuser open file too much!!!\n");
		sys_ofile[i].f_count = 0;
		iput(inode);
		return NULL;
	}

	user[user_sen].u_ofile[j] = i;  //xiao

	/*if APPEND, free the block of the file before*/
	if (openmode & FAPPEND)
	{/*
		for (i=0; i<inode->di_size/BLOCKSIZ+1; i++)
			bfree(inode->di_addr[i]);
		inode->di_size = 0;*/
	}

	return j; //xiao
}




