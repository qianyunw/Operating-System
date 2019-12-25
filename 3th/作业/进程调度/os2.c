s
#include<stdio.h>
#include<stdlib.h>
#define getpch(type) (type*)malloc(sizeof(type))
struct pcb
{
	char name[10];
	char state;
	int super;
	int ntime;
	int rtime;
	struct pcb *link;
}*ready = NULL, *p;

typedef struct pcb PCB;

void sort()
{
	PCB *first;
	first = ready;
	if(ready == NULL)
		ready = p;

	else{
		while(first -> link != NULL)
			first = first -> link;
		first -> link = p;

		PCB *most = ready;
		PCB *mostFormer = NULL;
		PCB *mostLater = NULL;
		PCB *second = ready;
		
		while(second -> link != NULL)
		{
			if((most -> super) < ((second -> link) -> super))
			{
				most = second -> link;
				mostFormer = second;
			}	
			second = second -> link;
		}
		mostLater = most -> link;

		if(mostFormer != NULL)
		{
			mostFormer -> link = mostLater;
			most -> link = ready;
			ready = most;
		}
		
	}
	return;
}

void input()
{
	int i, num;
	printf("\n Input the number of PCBs:");
	scanf("%d", &num);
	for(i = 0; i < num; i++)
	{
		printf("\n No.%d:", i);
		p = getpch(PCB);
		printf("\n Name:");
		scanf("%s", p->name);
		printf("\n Super:");
		scanf("%d", &p->super);
		printf("\n Ndtime:");
		scanf("%d", &p->ntime);
		printf("\n");
		p -> rtime = 0;
		p -> state = 'w';
		p -> link = NULL;
		sort();
	}
	return;
}

void disp(PCB *pr)
{
	printf("\n qname \t state \t super \t ndtime \t runtime \n");
	printf("|%s\t", pr->name);
	printf("|%c\t", pr->state);
	printf("|%d\t", pr->super);
	printf("|%d\t", pr->ntime);
	printf("|%d\t", pr->rtime);
	printf("\n");
	return;
}

void check()
{
	PCB* pr;
	printf("\n *************Current %s", p ->name);
	disp(p);
	pr = ready;
	printf("\n *************Ready");
	while(pr != NULL)
	{
		disp(pr);
		pr = pr -> link;
	}
	return;
}

void destory()
{
	printf("\n [%s] finished.\n", p -> name);
	free(p);
	return;
}

void running()
{
	(p -> rtime)++;
	(p -> super)--;
	if(p -> rtime == p -> ntime)
		destory();
	else
	{
		p -> state = 'w';
		sort();
	}
	return;
}

void main()
{
	int h = 0;
	char ch;
	input();
	ch = getchar();
	while(ready != NULL)
	{
		h++;
		printf("\n The execute number: %d\n", h);
		p = ready;
		ready = p -> link;
		p -> link = NULL;
		p -> state = 'R';
		check();
		running();
		printf("\n Press any key to continue...");
		ch = getchar();
	}
	printf("\n\n Os finished.\n");
	ch = getchar();
	return;
}




