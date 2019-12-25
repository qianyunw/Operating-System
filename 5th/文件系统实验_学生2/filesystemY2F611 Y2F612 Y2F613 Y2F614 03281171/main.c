
#include <stdio.h>
#include "filesys.h"


struct hinode hinode[NHINO];

struct dir dir;
struct file sys_ofile[SYSOPENFILE];
struct filsys filsys;
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
struct file *fd;   //xiao
struct inode *cur_path_inode;
int user_id;


char  disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];

main()
{int i;
	unsigned short ab_fd1,ab_fd2,ab_fd3,ab_fd4;
	//unsigned short bhy_fd1;
	char *buf;

	printf("\nDo you want to format the disk \n");
	/*
	if (getchar() == 'y')
	{
		printf("\nFormat will erase all context on the disk \nAre you sure!\n");
		if (getchar() == 'y')
			format();
	}
	*/
       format();
	install();
	_dir();
	login(2118,"abcd");
	user_id = 0;
	
	mkdir("a2118");
	_dir(); 
	chdir("a2118");
	_dir(); 
	
	ab_fd1 = creat(2118, "ab_file0.c", 01777);  //xiao
	_dir();
	if(ab_fd1 != -1)
	{
		buf = (char *)malloc(BLOCKSIZ*6+5);
		memset(buf, 0x11, sizeof(BLOCKSIZ*6+5));
		write(ab_fd1,buf,BLOCKSIZ*6+5);
		close(user_id,ab_fd1);
		free(buf);
	}
	mkdir("subdir");
	_dir(); 
	chdir("subdir");
	_dir(); 
	ab_fd2 = creat(2118, "ab_file1.c", 01777);  //xiao
	if(ab_fd2 != -1)
	{
		_dir(); 
		buf = (char *)malloc(BLOCKSIZ*4+20);
		write(ab_fd2,buf,BLOCKSIZ*4+20);
		close(user_id,ab_fd2);
		free(buf);
	}
	chdir("..");
	_dir(); 
	ab_fd3 = creat(2118, "ab_file2.c", 01777);  //xiao
	if(ab_fd3 != -1)
	{
		buf = (char *)malloc(BLOCKSIZ*10+255);
		write(ab_fd3,buf,BLOCKSIZ*10+255);
		close(user_id,ab_fd3);  //xiao
		free(buf);
		//_dir();			//}
	}

	delete("ab_file0.c");

	ab_fd4 = creat(2118, "ab_file4.c", 01777);  //xiao
	buf = (char *)malloc(BLOCKSIZ*8+300);
	write(ab_fd1,buf,BLOCKSIZ*8+300);
	close(user_id,ab_fd4);  //xiao
	free(buf);
	
	ab_fd3 = open(2118, "ab_file2.c", FAPPEND);  
	buf = (char *)malloc(BLOCKSIZ*3+100);
	write(ab_fd3,buf,BLOCKSIZ*3+100);
	close(user_id,ab_fd3);  //xiao
	free(buf);

	_dir();
	chdir("..");
	_dir();								//}
	
	//
	ab_fd3 = open(2118, "password", FREAD);
	read(ab_fd3, (char *)pwd, BLOCKSIZ);
	close(user_id,ab_fd3);
	printf("\ndata of \"password\" file as follow: \n");
	for(i = 0; i < PWDNUM; i ++)
	{
		if(pwd[i].p_uid != 0)
			printf("\t%d  %d   %s\n", pwd[i].p_uid, pwd[i].p_gid, pwd[i].password);
	}
	//
	
	logout(2118);
	halt();

	return 0;
}