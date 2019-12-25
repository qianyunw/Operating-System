// os_memory1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string> 
using namespace std;

struct Zone
{
	int startId;
	int length;
	string tag;
};

Zone Free[10];
Zone Busy[10];
int freeNext = 0;
int busyNext = 0;

void Init() {
	for (int i = 0; i < 10; i++) {
		struct Zone z{ 0, 0, "0"};
		Free[i] = z;
		Busy[i] = z;
	}
	struct Zone z { 10240, 102400, "1"};
	Free[0] = z;
	freeNext = 1;
	busyNext = 0;
	return;
}

void Show() {
	cout << "空闲区表：" << endl;
	cout << "起始地址 分区长度 标志" << endl;
	for (int i = 0; i < 10; i++) {
		cout << Free[i].startId << "\t" << Free[i].length << "\t" << Free[i].tag << "\t" << endl;
	}
	cout << "已分配表：" << endl;
	cout << "起始地址 分区长度 标志" << endl;
	for (int i = 0; i < 10; i++) {
		cout << Busy[i].startId << "\t" << Busy[i].length << "\t" << Busy[i].tag << "\t" << endl;
	}
	return;
}

void Allocate() {
	cout << "输入作业名：\t";
	string name;
	cin >> name;
	cout << "输入作业所需长度：\t";
	int length;
	cin >> length;

	int nice = -1;
	int sub[10];
	for (int i = 0; i < 10; i++) {
		sub[i] = Free[i].length - length;
		if (sub[i] >= 0) {
			nice = i;
		}
	}
	if (nice == -1) {
		cout << "内存不足，分配失败" << endl;
		return;
	}
	else {
		for (int i = 0; i < 10; i++) {
			if (sub[i] >= 0 && sub[i] < sub[nice]) {
				nice = i;
			}
		}
		if (sub[nice] == 0) {
			Busy[busyNext].startId = Free[nice].startId;
			Busy[busyNext].length = length;
			Busy[busyNext].tag = name;
			busyNext++;
			for (int j = nice; j < freeNext; j++) {
				Free[j] = Free[j+1];
			}
			freeNext--;
			cout << "分配成功，完美匹配" << endl;
		}
		else {
			Free[nice].length -= length;
			Busy[busyNext].startId = Free[nice].startId + sub[nice];
			Busy[busyNext].length = length;
			Busy[busyNext].tag = name;
			busyNext++;
			cout << "分配成功，留有碎片" << endl;
		}
		return;
	}
}


void Recycle() {
	cout << "输入要回收分区的作业名:\t";
	string name;
	cin >> name;

	int nice = -1;
	for (int i = 0; i < 10; i++) {
		if (name == Busy[i].tag) {
			nice = i;
			break;
		}
	}
	if (nice == -1) {
		cout << "未找到对应分区" << endl;
		return;
	}
	else {
		Free[freeNext] = Busy[nice];
		for (int j = nice; j < busyNext; j++) {
			Busy[j] = Busy[j + 1];
		}
		Free[freeNext].tag = "1";
		freeNext++;
		busyNext--;

		int currentFree = freeNext - 1;
		int start = Free[currentFree].startId;
		int end = Free[currentFree].startId + Free[currentFree].length;
		for (int i = 0; i < 10; i++) {
			if (start == (Free[i].startId + Free[i].length)) {
				Free[i].length += Free[currentFree].length;
				struct Zone z { 0, 0, "0" };
				Free[currentFree] = z;
				freeNext--;
				currentFree = i;
				start = Free[currentFree].startId;
				end = Free[currentFree].startId + Free[currentFree].length;
				cout << "合并于尾，";
				break;
			}
		}

		//失误：应当更新Free[current];
		for (int i = 0; i < 10; i++) {
			if (end == Free[i].startId) {
				Free[i].startId = start;
				Free[i].length += Free[currentFree].length;
				struct Zone z { 0, 0, "0" };
				Free[currentFree] = z;
				for (int j = 0; j < 9; j++) {
					if (Free[j].length == 0) {
						for(int m = j; m < 9; m++)
							Free[m] = Free[m + 1];
					}
				}
				freeNext--;
				cout << "合并于头，";
				break;
			}
		}
		cout << "回收成功" << endl;
		return;
	}
}

int main()
{
	Init();

	cout << "功能项(0 - 退出，1 - 分配主存；2 - 回收主存；3 - 显示主存)" << endl;
	cout << "选择功能项(0 - 3) :\t";
	int choice = 0;
	cin >> choice;

	while (choice != 0) {
		switch (choice)
		{
		case 1:
			Allocate();
			break;
		case 2:
			Recycle();
			break;
		case 3:
			Show();
			break;
		default:
			break;
		}

		cout << endl << "功能项(0 - 退出，1 - 分配主存；2 - 回收主存；3 - 显示主存)" << endl;
		cout << "选择功能项(0 - 3) :\t";
		cin >> choice;
	}
    return 0;
}

