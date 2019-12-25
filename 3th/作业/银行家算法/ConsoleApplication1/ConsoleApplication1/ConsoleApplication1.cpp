// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include<iostream> 
using namespace std;


int Available[20];
int Max[30][20];
int Allocation[30][20];
int Need[30][20];
int Requesti[20];
int Work[20];
int Finish[30];
int resourceNum;
int progressNum;
int requestNum;
bool IsContinue = true;


int FindProgress() {
	IsContinue = false;
	int i, j;
	for (i = 0; i < progressNum; i++) {
		if (Finish[i] == false) {
			for (j = 0; j < resourceNum; j++) {
				if (Need[i][j] > Work[j]) {
					break;
				}
			}
			if (j == resourceNum)
				IsContinue = true;
		}
		if (IsContinue == true)
			break;
	}
	return i;
}

void Run(int i) {
	int j;
	for (j = 0; j < resourceNum; j++) {
		Work[j] = Work[j] + Allocation[i][j];
	}
	Finish[i] = true;

	cout << i << ":";
	cout << "\t";
	for (j = 0; j < resourceNum; j++) {
		cout << Work[j] << " ";
	}
	cout << "\t\t";
	for (j = 0; j < resourceNum; j++) {
		cout << Need[i][j] << " ";
	}
	cout << "\t\t";
	for (j = 0; j < resourceNum; j++) {
		cout << Allocation[i][j] << " ";
	}
	cout << "\t\ttrue";
	cout << endl;
}

bool IsSafe() {
	int i;
	for (i = 0; i < resourceNum; i++) {
		if (Finish[i] == false)
			break;
	}
	if (i == resourceNum) {
		return true;
	}
	else
		return false;
}

bool SafeAlgorithm() {
	for (int i = 0; i < resourceNum; i++)
		Work[i] = Available[i];
	for (int i = 0; i < progressNum; i++)
		Finish[i] = false;

	cout << "**********************************SafeAlgorithm\n";
	cout << "\tWork\t" << "\tNeed\t" << "\tAllocation" << "\tFinish" << endl;
	while (IsContinue == true) {
		int i = FindProgress();
		if (IsContinue == true) {
			Run(i);
		}
	}
	if (IsSafe() == true) {
		cout << "Yes, you can continue\n";
		return true;
	}
	else {
		cout << "Sorry, you can't continue\n";
		return false;
	}
}

int main()
{

	cout << "Please input your progressNum";
	cin >> progressNum;
	cout << "Please input your resourceNum";
	cin >> resourceNum;
	int i, j;
	for (i = 0; i < progressNum; i++) {
		cout << "Max[" << i << "]:\t";
		for (j = 0; j < resourceNum; j++) {
			cin >> Max[i][j];
		}
	}
	for (i = 0; i < progressNum; i++) {
		cout << "Allocation[" << i << "]:\t";
		for (j = 0; j < resourceNum; j++) {
			cin >> Allocation[i][j];
		}
	}
	for (i = 0; i < progressNum; i++) {
		for (j = 0; j < resourceNum; j++) {
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}

	char ch = 'a';
	while (ch != EOF) {
		IsContinue = true;
		cout << "Please input your Available[j]:\t";
		for (j = 0; j < resourceNum; j++) {
			cin >> Available[j];
		}

		cout << "Please input your requestNum:\t";
		cin >> requestNum;

		cout << "Please input your Requesti[j]:\t";
		for (j = 0; j < resourceNum; j++) {
			cin >> Requesti[j];
		}

		cout << "**********************************begin\n";
		cout << "\tMax\t" << "\tAllocation" << "\tNeed" << endl;
		for (i = 0; i < progressNum; i++) {
			cout << i << ":";
			cout << "\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Max[i][j] << " ";
			}
			cout << "\t\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Allocation[i][j] << " ";
			}
			cout << "\t\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Need[i][j] << " ";
			}
			cout << endl;
		}


		i = requestNum;
		for (int j = 0; j < resourceNum; j++) {
			if (Requesti[j] > Need[i][j]) {
				cout << "Error";
			}
		}
		for (int j = 0; j < resourceNum; j++) {
			if (Requesti[j] > Available[j]) {
				cout << "Wait(Pi)";
			}
		}
		if (IsContinue == true) {

		}

		for (int j = 0; j < resourceNum; j++) {
			Available[j] = Available[j] - Requesti[j];
			Allocation[i][j] = Allocation[i][j] + Requesti[j];
			Need[i][j] = Need[i][j] - Requesti[j];
		}
		if (SafeAlgorithm() == false) {
			for (int j = 0; j < resourceNum; j++) {
				Available[j] = Available[j] + Requesti[j];
				Allocation[i][j] = Allocation[i][j] - Requesti[j];
				Need[i][j] = Need[i][j] + Requesti[j];
			}
		}

		cout << "**********************************end\n";
		cout << "\tMax\t" << "\tAllocation" << "\tNeed" << endl;
		for (i = 0; i < progressNum; i++) {
			cout << i << ":";
			cout << "\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Max[i][j] << " ";
			}
			cout << "\t\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Allocation[i][j] << " ";
			}
			cout << "\t\t";
			for (j = 0; j < resourceNum; j++) {
				cout << Need[i][j] << " ";
			}
			cout << endl;
		}
		ch = getchar();
	}
    return 0;
}
