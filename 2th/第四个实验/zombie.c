#include<sys/types.h>
#include<unistd.h>
main()
{ 
pid_t pid;
pid=fork();
if pid<0
   printf("����ʧ�ܣ�\n");
else if (pid==0)
   wait(0);
else 
   sleep(60);
   wait(NULL);
} 