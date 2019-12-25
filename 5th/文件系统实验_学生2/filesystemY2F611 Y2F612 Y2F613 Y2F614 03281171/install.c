

#include <stdio.h>
#include <string.h>
#include "filesys.h"

install()
{
	int i,j;

	/* 0. open the file column*/
	/*
	fd = fopen("filesystem", "w+r+b");
	if (fd == NULL)   //xiao
	{
		printf("\nfilesys can not be loaded\n");
		exit(0);
	}
	*/

	/* 1. read the filsys from the superblock*/   //xiao ???
	/*
	fseek(fd, BLOCKSIZ, SEEK_SET);
	fwrite(&filsys, 1, sizeof(struct filsys), fd);
	*/
	//memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));

	/* 2. initialize the inode hash chain*/
	for (i = 0; i < NHINO; i++)
		hinode[i].i_forw = NULL;

	/*3. initialize the sys_ofile*/
	for (i = 0; i < SYSOPENFILE; i++)
	{
		sys_ofile[i].f_count = 0;
		sys_ofile[i].f_inode = NULL;
	}

	/*4. initialize the user*/
	for (i = 0; i < USERNUM; i++)
	{
		user[i].u_uid = 0;
		user[i].u_gid = 0;
		for (j = 0; j < NOFILE; j++)
			user[i].u_ofile[j] = SYSOPENFILE+1;
	}

	/*5. read the main directory to initialize the dir*/
	cur_path_inode = iget(2);
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);//xiao 2-->4

	for (i=0; i < DIRNUM; i++)
	{
		strcpy(dir.direct[i].d_name,"             ");
		dir.direct[i].d_ino = 0;
	}
 
	for (i=0; i<dir.size/(BLOCKSIZ/(DIRSIZ+4)); i++)   /*dir.size/(BLOCKSIZ/(DIRSIZ+4)): 目录表所占用的物理块数 */
	{ 
		/*
		fseek(fd,DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], SEEK_SET);
		fread(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i], 1, BLOCKSIZ, fd);
		*/
		memcpy(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i], 
		disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], BLOCKSIZ); //{}disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);
	}

	/*
	fseek(fd, DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], SEEK_SET);
	fread(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i], 1, cur_path_inode->di_size % BLOCKSIZ, fd);
	*/
	memcpy(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i], 
		disk + DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], (dir.size - BLOCKSIZ/(DIRSIZ+4) * i) * (DIRSIZ+4));		//{}disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);
	return 0;
	
}

