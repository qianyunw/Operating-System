#include <stdio.h>
#include "filesys.h"

int login(unsigned short uid, char *passwd)
{
	int i,j;

	for (i=0; i<PWDNUM; i++)
	{
		if ((uid == pwd[i].p_uid) && (!strcmp(passwd, pwd[i].password)))
		{
			for (j=0; j<USERNUM; j++)   //xiao
				if (user[j].u_uid == 0) 
					break;
 
			if (j == USERNUM)
			{
				printf("\ntoo much user in the systemm, waited to login\n");
				return 0;
			}
			else
			{
				user[j].u_uid = uid;
				user[j].u_gid = pwd[i].p_gid; 
				user[j].u_default_mode = DEFAULTMODE;
			}
			break;
		}
	}

	if (i == PWDNUM)
	{
		printf("\nincorrect password\n");
		return 0;
	}
	else
		return 1;
}


int logout(unsigned short uid)
{
	int i,j,sys_no;
	struct inode *inode;

	for (i=0; i<USERNUM; i++)
		if (uid == user[i].u_uid)
			break;

	if (i == USERNUM)
	{
		printf("\nno such a file\n");
		return NULL;
	}

	for (j=0; j<NOFILE; j++)
	{
		if (user[i].u_ofile[j] != SYSOPENFILE+1)
		{
			/* iput the inode free the sys_ofile and clear the user_ofile*/
			sys_no = user[i].u_ofile[j];
			inode = sys_ofile[sys_no].f_inode;
			iput(inode);
			sys_ofile[sys_no].f_count--;
			user[i].u_ofile[j] = SYSOPENFILE+1;
		}
	}

	return 1;
}




































