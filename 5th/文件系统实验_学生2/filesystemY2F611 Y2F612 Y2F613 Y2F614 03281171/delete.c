#include <stdio.h>
#include "filesys.h"

delete(char *filename)
{
	unsigned int dinodeid;
	struct inode *inode;
	int i;

	dinodeid = namei(filename);
	if (dinodeid != 0)
	{
		inode = iget(dinodeid);
		inode->di_number--;
		iput(inode);

		//
		for(i = 0; i < dir.size; i ++)
		{
			if(!strcmp(dir.direct[i].d_name, filename))
				break;
		}
		for(; i < dir.size - 1; i ++)
		{
			strcpy(dir.direct[i].d_name, dir.direct[i + 1].d_name);
			dir.direct[i].d_ino = dir.direct[i + 1].d_ino;
		}
		dir.direct[i].d_ino = 0;
		dir.size --;
		//
		return 1;
	}
	return 0;
}











