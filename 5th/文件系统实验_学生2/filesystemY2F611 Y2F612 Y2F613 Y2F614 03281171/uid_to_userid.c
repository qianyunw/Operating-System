#include <stdio.h>
#include "filesys.h"

int uid_to_userid(unsigned short uid)
{
	int i;
	for (i = 0; i < USERNUM; i++)
	{
		if(user[i].u_uid == uid)
			break;
	}
	if(i == USERNUM)
	{
		printf("\n have no correspond uid.\n ");
		return USERNUM + 1;
	}
	else
	{
		return i;
	}
}