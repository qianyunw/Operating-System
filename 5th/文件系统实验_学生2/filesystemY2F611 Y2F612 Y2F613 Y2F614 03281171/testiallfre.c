#include <stdio.h>
#include "filesys.h"

void testiallfre()
{
	
	struct inode *temp_inode;
	struct dinode temp_dinode;
	int n;
	int i;
	do
	{
		scanf("%d", &n);
		switch(n)
		{
		case 0:
			temp_inode = ialloc();
			printf("%s%d", "得到的i节点为 : ", temp_inode->i_ino);
			iput(temp_inode);			
			break;
		case 1:
			printf("输入要回收的i节点号 : ");
			scanf("%d", &n);
			ifree(n);
			break;
		case 2:
			printf("已分配的i节点:\n");
			for(i = 0; i < MAX_DINODEID; i ++)
			{
				memcpy(&temp_dinode, disk + DINODESTART + i * DINODESIZ, DINODESIZ);	
				if(temp_dinode.di_mode != DIEMPTY)
				{
					printf("\t%d\n", i);
				}
			}
			break;
		case 3:
			printf("filsys.s_rinode : %d\n", filsys.s_rinode);
			printf("空闲i节点栈内容为 : ");
			for(i = filsys.s_pinode; i < NICINOD; i ++)
				printf("%d ", filsys.s_inode[i]);
			break;
		}
	}while(1);
	/*for(int i = 0; i < dir.size; i ++)
	{
		printf("%12s, %d\n", dir.direct[i].d_name, dir.direct[i].d_ino);
	}*/

}