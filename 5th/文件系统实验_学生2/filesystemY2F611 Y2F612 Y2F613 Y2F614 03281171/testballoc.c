#include <stdio.h>
#include "filesys.h"

void testballoc()
{
	int i, n;
	do
	{
		scanf("%d", &n);
		switch(n)
		{
		case 0:
			n = balloc();
			printf("得到的节点为 : %d\n", n);
			break;
		case 1:
			printf("输入要回收的物理块号 : ");
			scanf("%d", &n);
			bfree(n);
			break;
		case 2:
			printf("空闲块栈内容为 : \n");
			for(i = filsys.s_pfree; i < NICFREE; i ++)
			{
				printf("\t%d", filsys.s_free[i]);
			}
			printf("\n");

			break;
		}
	}while(1);
}

