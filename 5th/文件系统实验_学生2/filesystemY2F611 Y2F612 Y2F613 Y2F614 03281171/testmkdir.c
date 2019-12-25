#include <stdio.h>
#include "filesys.h"

void testmkdir()
{
	char dirname[DIRSIZ];
	do
	{	
		printf("ÊäÈëÄ¿Â¼Ãû :\n");
		scanf("%s", &dirname);
		mkdir(dirname);
		_dir();
	}while(1);
}



