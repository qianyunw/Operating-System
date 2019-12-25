// os_memory1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string> 
using namespace std;
//ctrl+F5:：不闪退调试

#define N 64

struct page
{
	int lNumber; //页号 
	int pNumber; //物理块号 
	int dNumber; //在磁盘上的位置 
	int write; //修改标志 
	int flag; //存在标志 
}page[N];

int pageNumber;
int memoryNumber; 
int eliminatNumber;
int p[N];

void Init() {
	cout << "创建页表（-1结束）" << endl;
	int dNumber;
	cout << "page[" << pageNumber << "].dNumber:\t";
	cin >> dNumber;
	while (dNumber != -1) {
		page[pageNumber].dNumber = dNumber;
		page[pageNumber].lNumber = pageNumber;
		pageNumber++;
		cout << "page[" << pageNumber << "].dNumber:\t";
		cin >> dNumber;
	}

	cout << "输入主存块号，块数 <="<<pageNumber<<"（-1结束）"<< endl;
	int pNumber;
	cout << "memory[" << memoryNumber << "]:\t";
	cin >> pNumber;
	while (pNumber != -1) {
		page[memoryNumber].pNumber = pNumber;
		page[memoryNumber].flag = 1;
		memoryNumber++;
		cout << "memory[" << memoryNumber << "]:\t";
		cin >> pNumber;
		p[memoryNumber] = memoryNumber;
	}
	if (memoryNumber > 4) {
		for (int i = 4; i < pageNumber; i++) {
			page[i].pNumber = 0;
			page[i].flag = 0;
			p[memoryNumber] = -1;
		}
		memoryNumber = 4;
	}
	return;
}

void Show() {
	cout << "\nLet me show you some secrets：\n";
	cout << "\t\tlNumber\tpNumber\tflag\twrite\tdNumber\n";
	for (int i = 0; i < pageNumber; i++) {
		cout << "page[" << i << "]:\t" << page[i].lNumber << "\t" << page[i].pNumber << "\t" << page[i].flag << "\t" << page[i].write << "\t" << page[i].dNumber << endl;
	}
	cout << "\np[m]:\t";
	for (int i = 0; i < memoryNumber; i++) {
		cout <<p[i]<<"\t";
	}
	return;
}

void RunCommand() {
	cout << "\n\n0 - 不修改，1 - 修改:\t";
	int isChange;
	cin >> isChange;
	cout << "逻辑地址：\t";
	int address;
	cin >> address;
	while (isChange == 0 || isChange == 1) {
		int lNumber = address / 1024;
		int leftNumber = address % 1024;
		if (lNumber < pageNumber) {
			if (page[lNumber].flag == 1) {
				cout << address << "'s real address is:\t" << page[lNumber].pNumber * 1024 + leftNumber;
				if (isChange)
					page[lNumber].write = 1;
			}
			else {
				int lNumber = address / 1024;
				int leftNumber = address % 1024;
				int EliminatPage = p[eliminatNumber % memoryNumber];
				p[eliminatNumber % memoryNumber] = EliminatPage;
				page[lNumber].flag = 1;
				page[lNumber].pNumber = page[EliminatPage].pNumber;
				if (isChange)
					page[lNumber].write = 1;
				page[EliminatPage].flag = 0;
				page[EliminatPage].write = 0;
				page[EliminatPage].pNumber = 0;
				p[eliminatNumber % memoryNumber] = lNumber;
				eliminatNumber++;

				cout << "访问的" << lNumber << "页不在内存，发生缺页中断\n";
				cout << "淘汰主存中第" << EliminatPage << "页,从磁盘第" << page[lNumber].pNumber << "块调入页" << lNumber;
				cout << "\n对应的逻辑地址是" << page[lNumber].pNumber * 1024 + leftNumber;
			}
		}
		else
			cout << "访问的" << lNumber << "页不存在，发生严重缺页中断\n";
		Show();
		cout << "\n\n0 - 不修改，1 - 修改:\t";
		cin >> isChange;
		cout << "逻辑地址：\t";
		cin >> address;
	}
	return;
}


int main()
{
	Init();
	Show();
	RunCommand();
    return 0;
}

