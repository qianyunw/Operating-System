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
			printf("�õ��Ľڵ�Ϊ : %d\n", n);
			break;
		case 1:
			printf("����Ҫ���յ������� : ");
			scanf("%d", &n);
			bfree(n);
			break;
		case 2:
			printf("���п�ջ����Ϊ : \n");
			for(i = filsys.s_pfree; i < NICFREE; i ++)
			{
				printf("\t%d", filsys.s_free[i]);
			}
			printf("\n");

			break;
		}
	}while(1);
}

