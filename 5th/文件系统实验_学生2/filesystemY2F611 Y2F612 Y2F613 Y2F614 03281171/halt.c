#include <stdio.h>
#include <stdlib.h>
#include "filesys.h"


halt()
{
	//struct inode *inode;
	int i,j;

	/*1. write back the current dir */
	chdir("..");
	iput(cur_path_inode);

	/*2. free the u_ofile and sys_ofile and inode*/
	for (i=0; i < USERNUM; i++)
	{
		if (user[i].u_uid != 0)
		{
			for (j=0; j<NOFILE;j++)
			{
				if (user[i].u_ofile[j] != SYSOPENFILE + 1)
				{
					close(i, user[i].u_ofile[j]);
					user[i].u_ofile[j] = SYSOPENFILE + 1;
				}
			}
		}
	}

	/*3. write back the filesys to the disk*/
	/*
	fseek(fd, BLOCKSIZ, SEEK_SET);
	fwrite(&filsys, 1, sizeof(struct filsys), fd);
	*/
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));

	/*4. close the file system column*/
	//fclose(fd);

	/*5. say GOOD BYE to all the user*/
	printf("\nGood Bye. See You Next Time. Please turn off the switch\n");
	exit(0);
	
}





























