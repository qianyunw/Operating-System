// os_memory1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string> 
using namespace std;
//ctrl+F5:�������˵���

#define N 64

struct page
{
	int lNumber; //ҳ�� 
	int pNumber; //������ 
	int dNumber; //�ڴ����ϵ�λ�� 
	int write; //�޸ı�־ 
	int flag; //���ڱ�־ 
}page[N];

int pageNumber;
int memoryNumber; 
int eliminatNumber;
int p[N];

void Init() {
	cout << "����ҳ��-1������" << endl;
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

	cout << "���������ţ����� <="<<pageNumber<<"��-1������"<< endl;
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
	cout << "\nLet me show you some secrets��\n";
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
	cout << "\n\n0 - ���޸ģ�1 - �޸�:\t";
	int isChange;
	cin >> isChange;
	cout << "�߼���ַ��\t";
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

				cout << "���ʵ�" << lNumber << "ҳ�����ڴ棬����ȱҳ�ж�\n";
				cout << "��̭�����е�" << EliminatPage << "ҳ,�Ӵ��̵�" << page[lNumber].pNumber << "�����ҳ" << lNumber;
				cout << "\n��Ӧ���߼���ַ��" << page[lNumber].pNumber * 1024 + leftNumber;
			}
		}
		else
			cout << "���ʵ�" << lNumber << "ҳ�����ڣ���������ȱҳ�ж�\n";
		Show();
		cout << "\n\n0 - ���޸ģ�1 - �޸�:\t";
		cin >> isChange;
		cout << "�߼���ַ��\t";
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

