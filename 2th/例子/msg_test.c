#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h> 

struct msgbuf
{
	long mtype;
	char mtext[1];
};
 
int main()
{
	int msqid;
	struct msqid_ds info;
	struct msgbuf buf;
	msqid=msgget(IPC_PRIVATE,0777|IPC_CREAT);
	printf("0777|IPC_CREAT:%d\n",0777|IPC_CREAT);
	buf.mtype=1;
	buf.mtext[0]=1;
	msgsnd(msqid,&buf,1,0);
	msgctl(msqid,IPC_STAT,&info);
	printf("read-write:%o\n",info.msg_perm.mode);
	system("ipcs -q");
	msgctl(msqid,IPC_RMID,0);
	exit(0);
}