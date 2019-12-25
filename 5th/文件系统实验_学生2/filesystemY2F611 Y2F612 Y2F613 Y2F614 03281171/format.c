#include <stdio.h>
#include "filesys.h"
format()
{
	struct inode *inode;
	struct direct dir_buf[BLOCKSIZ/(DIRSIZ+4)];				
	//struct pwd passwd[BLOCKSIZ/(PWDSIZ+4)];
	struct pwd passwd[32];
	/*
		{
			{2116,03,"dddd"},
			{2117,03,"bbbb"},
			{2118,04,"abcd"},
			{2119,04,"cccc"},
			{2120,03,"eeee"},
		};
	*/

//	struct filsys;  xiao
	unsigned int block_buf[BLOCKSIZ/sizeof(int)];
	//char *buf;
	int i,j;

	memset(disk, 0x00, ((DINODEBLK+FILEBLK+2)*BLOCKSIZ));

	/* 0.initialize the passwd */
	passwd[0].p_uid = 2116;						//?
	passwd[0].p_gid = 03;
	strcpy(passwd[0].password, "dddd");

	passwd[1].p_uid = 2117;
	passwd[1].p_gid = 03;
	strcpy(passwd[1].password, "bbbb");

	passwd[2].p_uid = 2118;
	passwd[2].p_gid = 04;
	strcpy(passwd[2].password, "abcd");  

	passwd[3].p_uid = 2119;
	passwd[3].p_gid = 04;
	strcpy(passwd[3].password, "cccc");

	passwd[4].p_uid = 2120;
	passwd[4].p_gid = 05;
	strcpy(passwd[4].password, "eeee");

	/* 1.creat the main directory and its sub dir etc and the file password */

	inode = iget(0);   /* 0 empty dinode id*/
	inode->di_mode = DIEMPTY;
	iput(inode);

	inode = iget(1);   /* 1 main dir id*/
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0; /*block 0# is used by the main directory*/
	
	strcpy(dir_buf[0].d_name,"..");
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 1;
	strcpy(dir_buf[2].d_name,"etc");
	dir_buf[2].d_ino = 2;

	/*
	fseek(fd, DATASTART, SEEK_SET);
	fwrite(dir_buf,1,3*(DIRSIZ+4),fd);   //xiao ???  2-->4
	*/
	memcpy(disk+DATASTART, &dir_buf, 3*(DIRSIZ+4));
	//memcpy(disk+DATASTART, &dir_buf[1], 2*(DIRSIZ+4));
	iput(inode);

	inode = iget(2);  /* 2  etc dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * (DIRSIZ + 4);
	inode->di_addr[0] = 1; /*block 1# is used by the etc directory*/
	
	strcpy(dir_buf[0].d_name,"..");
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name,".");
	dir_buf[1].d_ino = 2;
	strcpy(dir_buf[2].d_name,"password");
	dir_buf[2].d_ino = 3;

	/*
	fseek(fd, DATASTART+BLOCKSIZ*1, SEEK_SET);
	fwrite(dir_buf,1,3*(DIRSIZ+4),fd);
	*/
	memcpy(disk+DATASTART+BLOCKSIZ*1, dir_buf, 3*(DIRSIZ+4));
	iput(inode);


	inode = iget(3);  /* 3  password id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2; /*block 2# is used by the password file*/

	for (i=5; i<PWDNUM; i++)
	{
		passwd[i].p_uid = 0;
		passwd[i].p_gid = 0;
		strcpy(passwd[i].password, "            ");  // PWDSIZ " "
	}
	
	//added by xiao for a while
	memcpy(pwd, passwd, 32*sizeof(struct pwd));
	/*
	fseek(fd, DATASTART+2*BLOCKSIZ, SEEK_SET);
	fwrite(passwd,1,BLOCKSIZ, fd);
	*/
	memcpy(disk+DATASTART+BLOCKSIZ*2, passwd, BLOCKSIZ);
	iput(inode);

	/*2. initialize the superblock */

	filsys.s_isize = DINODEBLK;
	filsys.s_fsize = FILEBLK;

	filsys.s_ninode = DINODEBLK * BLOCKSIZ/DINODESIZ - 4;
	filsys.s_nfree = FILEBLK - 3;

	for (i=0; i < NICINOD; i++)
	{
		/* begin with 4,    0,1,2,3, is used by main,etc,password */
		filsys.s_inode[i] = 4 + i;									//?
	}

	filsys.s_pinode = 0;
	filsys.s_rinode = NICINOD + 4; 

	block_buf[NICFREE-1] = FILEBLK+1;  /*FILEBLK+1 is a flag of end*/
	for (i=0; i<NICFREE-1; i++)							//?511? 512
		block_buf[NICFREE-2-i] = FILEBLK-i;

	/* block_buf[0]--> FILBLK-48
	    block_buf[47]--> FILBLK-1
	    block_buf[48]--> FILBLK*/

	/*
	fseek(fd, DATASTART+BLOCKSIZ*(FILEBLK-NICFREE-1), SEEK_SET);
	fwrite(block_buf,1,BLOCKSIZ, fd);
	*/    
	//BLOCKSIZ*(FILEBLK-NICFREE-1)= 0x39a00
	//memcpy(disk+DATASTART+BLOCKSIZ*(FILEBLK-NICFREE-1), block_buf, BLOCKSIZ);

	memcpy(disk+DATASTART+BLOCKSIZ*(FILEBLK-NICFREE+1), block_buf, BLOCKSIZ);

	//for (i=FILEBLK-NICFREE-1; i>2; i-=NICFREE)
	for (i=FILEBLK-NICFREE+1; i>=NICFREE; i-=NICFREE)			//{}for (i=FILEBLK-NICFREE+1; i>2; i-=NICFREE)
	{
		for (j=0; j<NICFREE;j++)
			block_buf[NICFREE-j-1] = i-j;

		/*
		fseek(fd, DATASTART+BLOCKSIZ*(i-1), SEEK_SET);
		fwrite(block_buf, 1, BLOCKSIZ, fd);
		*/
		memcpy(disk+DATASTART+BLOCKSIZ*(i-NICFREE), block_buf, BLOCKSIZ);//{}memcpy(disk+DATASTART+BLOCKSIZ*(i-1), block_buf, BLOCKSIZ);
	}
/*
	j = 1;
	for (i=i; i>2; i--)          //xiao ????????????????????????????
	{
		filsys.s_free[NICFREE+i-j] = i;
	}

	filsys.s_pfree = NICFREE - j;
	*/
														//{}j = NICFREE+i;
	for (j=i; i>2; i--)									//{}for (i=j; i>2; i--)
	{
		filsys.s_free[NICFREE-1 + (i - j)] = i;			//}}filsys.s_free[NICFREE+i-j-1] = i; 
	}
 
	filsys.s_pfree = NICFREE-1+(i-j)+1;					//}}filsys.s_pfree = NICFREE - j+2; 
														//}}filsys.s_pinode = 0;

	/* 
	fseek(fd, BLOCKSIZ, SEEK_SET);
	fwrite(&filsys, 1, sizeof(struct filsys), fd);
	*/
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));

	return 0; 
	
}
