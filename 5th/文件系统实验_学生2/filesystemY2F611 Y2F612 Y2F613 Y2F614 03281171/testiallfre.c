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
			printf("%s%d", "�õ���i�ڵ�Ϊ : ", temp_inode->i_ino);
			iput(temp_inode);			
			break;
		case 1:
			printf("����Ҫ���յ�i�ڵ�� : ");
			scanf("%d", &n);
			ifree(n);
			break;
		case 2:
			printf("�ѷ����i�ڵ�:\n");
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
			printf("����i�ڵ�ջ����Ϊ : ");
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