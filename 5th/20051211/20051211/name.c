#include <stdio.h>
#include <string.h>
#include "filesys.h"

unsigned int namei(char *name)
{
	int  i,notfound=1;  //xiao ??? varible notfound is useless

	for (i=0; ((i<dir.size) && (notfound)); i++)
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))
			//return i;   /**/
			return (dir.direct[i].d_ino);  //xiao 

	return NULL;  //not find
} 
   

unsigned short iname(char *name)
{
	int i, notfound = 1;

	for (i=0; ((i<DIRNUM) && (notfound)); i++)
		if (dir.direct[i].d_ino == 0)
		{
			notfound = 0;
			break;
		}

	if (notfound)
	{
		printf("\nThe current directory is full!!!\n");
		return 0;
	}
	else
	{
		//strcpy(name, dir.direct[i].d_name);   modified by xiao 
		strcpy( dir.direct[i].d_name, name); 
		return i;
	} 
}










































