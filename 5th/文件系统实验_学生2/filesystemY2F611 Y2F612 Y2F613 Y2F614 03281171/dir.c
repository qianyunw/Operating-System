#include <stdio.h>
#include <string.h>
#include "filesys.h"


_dir()
{
	unsigned int di_mode;
	unsigned int single_mode;
	int i,one;
	int j,k;          //xiao  
	struct inode *temp_inode;

	printf("\nCURRENT DIRECTORY :\n");
	for(i = 0; i < dir.size; i ++)
	{
		if(dir.direct[i].d_ino != DIEMPTY) 
		{
			printf("%-14s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			//di_mode = temp_inode->di_mode;  by xiao
			// 是否为目录
			di_mode = temp_inode ->di_mode;
			one = di_mode / 01000;
			if(one == 2)				//若为目录
				printf("d");
			else if(one == 1)
				printf("-");
			di_mode = di_mode & 00777;

			for(j = 0; j < 9; j ++)
			{	
				if(j % 3 == 0)
				{
					single_mode = di_mode & 07;
					di_mode = di_mode >> 3;
				}
				one = single_mode % 2;
				single_mode = single_mode / 2;
				if(one)
				{
					switch(j % 3)
					{
					case 0:
						printf("r");
						break;
					case 1:
						printf("w");
						break;
					case 2:
						printf("x");
						break;
					}
				}
				else
					printf("-");
			}
			//if (temp_inode->di_mode && DIFILE == 1)
			if (temp_inode->di_mode & DIFILE)   //by xiao
			{
				printf(" %10ld ", temp_inode->di_size);
				printf("block chain:");

				for(k = 0; k < temp_inode->di_size/BLOCKSIZ; k ++)	//{}for(k = 0; k < temp_inode->di_size/BLOCKSIZ + 1; k ++)
					printf("%4d", temp_inode->di_addr[k]);
				if(temp_inode ->di_size % BLOCKSIZ != 0)
					printf("%4d", temp_inode->di_addr[k]);
				printf("\n");
			}
			else
				printf("%+10s","<DIR>\n");
			iput(temp_inode);
		} 
	}
	return 0;
}

mkdir(char *dirname)
{
	int dirid, dirpos;
	struct inode *inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	unsigned int block;

	dirid = namei(dirname);
	if(dirid != 0)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("\n%s directory already existed!!", dirname); //xiao
		else
			printf("\n%s is a file name, can't creat a dir the same name", dirname);
		iput(inode);
		return -1;
	}
	dirpos = iname(dirname);
	if(dirpos == 0)
	{
		return -1;
	}
	inode = ialloc();
	if(inode == NULL)
	{
		return -1;
	}
	//inode->i_ino = dirid;
	
	dir.direct[dirpos].d_ino = inode->i_ino;
	dir.size ++;
	cur_path_inode->di_size += DIRSIZ + 4;

	/*fill the new dir buf*/
	strcpy(buf[0].d_name,".");
	//buf[0].d_ino = dirid;  //by xiao
	buf[0].d_ino = inode->i_ino;
	strcpy(buf[1].d_name, "..");
	buf[1].d_ino = cur_path_inode->i_ino;

	block = balloc();
	/*
	fseek(fd, DATASTART+block*BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fd);
	*/
	memcpy(disk+DATASTART+block*BLOCKSIZ, buf, (DIRSIZ + 4) * 2); //{}memcpy(disk+DATASTART+block*BLOCKSIZ, buf, BLOCKSIZ);

	inode->di_number = 1; 
	inode->di_mode = user[user_id].u_default_mode | DIDIR;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_size = 2 * (DIRSIZ + 4);
	inode->di_addr[0] = block;

	iput(inode);

	return 0;
	
}


chdir(char *dirname)
{
	unsigned int dirid;
	struct inode *inode;
	// unsigned short block;
	int i, low=0, high = 0;

	dirid = namei(dirname);
	if (dirid == 0)
	{
		printf("\n%s does not existed\n", dirname);
		return -1;
	}
	inode = iget(dirid); 
/*
	if (!access(user_id, inode, user[user_id].u_default_mode))
	{
		printf("\nhas not access to the directory %s",dirname);
		iput(inode); 
		return;
	}
*/
	/*pack the current directory*/
#if 0
	for (i = 0; i < dir.size; i ++)
	{
		for (; j < DIRNUM; j ++)
			if (dir.direct[j].d_ino == 0) 
				break;
		memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ+4);  //xiao
		dir.direct[j].d_ino = 0;
	}

	/* write back the current directory */
	for (i=0; i<cur_path_inode->di_size/BLOCKSIZ+1; i++)
		bfree(cur_path_inode->di_addr[i]);

	for (i=0; i<dir.size; i+=BLOCKSIZ/(DIRSIZ+4))
	{
		block = balloc();
		cur_path_inode->di_addr[i] = block;
		/*
		fseek(fd, DATASTART+block*BLOCKSIZ, SEEK_SET);
		fwrite(&dir.direct[i], 1, BLOCKSIZ, fd);
		*/
		memcpy(disk+DATASTART+block*BLOCKSIZ, &dir.direct[i], BLOCKSIZ);
	}
#endif
	cur_path_inode->di_size = dir.size * (DIRSIZ+4);
	iput(cur_path_inode);

	cur_path_inode = inode;
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);
	/*read the change dir from disk*/
	
	for (i =0; i < inode->di_size/BLOCKSIZ; i++)
	{
		/*
		fseek(fd, DATASTART+inode->di_addr[i]*BLOCKSIZ, SEEK_SET);
		fread(&dir.direct[j], 1, BLOCKSIZ, fd);
		*/
		memcpy(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i], 
			disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], BLOCKSIZ);
	}
	memcpy(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i], 
		disk + DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], (dir.size - BLOCKSIZ/(DIRSIZ+4) * i) * (DIRSIZ+4));	

	//added by xiao
	
	
	for (i = dir.size; i < DIRNUM; i++) 
		dir.direct[i].d_ino = 0;
	
	//end by xiao

	return 0;  
} 

















