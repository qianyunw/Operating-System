#include <sys/types.h> 
#include <sys/msg.h>
#include <sys/ipc.h>
#define MSGKEY 75         /*���干�����ؼ���*/

struct msgform
{
	long mtype;
	char mtext[256]; 
};

main()
{
	struct msgform msg;
	int msgqid,pid,*pint;
	msgqid=msgget(MSGKEY,0777);      /*������Ϣ����*/
	pid=getpid();
	pint=(int *)msg.mtext;
	*pint=pid;
	msg.mtype=1;                     /*ָ����Ϣ����*/
	msgsnd(msgqid,&msg,sizeof(int),0);       /*��msgqid������Ϣmsg*/
	msgrcv(msgqid,&msg,256,pid,0);        /*�������Է��������̵���Ϣ*/
	printf("client:receive from pid %d\n",*pint);
}

