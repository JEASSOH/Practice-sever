#include<iostream>
#include<vector>
#include<chrono>
#include <windows.h>
#include <process.h>
#include <tchar.h>

using namespace std;
const int MaxCount = 300000;
#define MAX_THREADS (1024*10)
DWORD cntOfThread = 0;

unsigned int WINAPI ThreadProc(LPVOID IpParam)
{
	DWORD threadNum = (DWORD) IpParam;
	while (1)
	{
		IsPrimeNumber();
	}

	return 0;
	
}



bool IsPrimeNumber(int number)
{
	if (number == 1)
		return false;
	if (number == 2 || number == 3)
		return true;
	for (int i = 2; i < number - 1; i++)
	{
		if ((number % i) == 0)
			return false;
	}
	return true;
}

void PrintNumbers(const vector<int>& primes)
{
	for (int v : primes)
	{
		cout << v << endl;
	}
}

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	// ���� ������ �ִ� ������ ������ ����

	while (1)
	{

		hThread[cntOfThread] = (HANDLE)
			_beginthreadex(
				NULL,					   // ����Ʈ ���� ������.
				0,				           // ����Ʈ ���� ������.
				ThreadProc,				   // ������ main �Լ�(������ �Լ�) ����.
				(LPVOID)cntOfThread,       // ������ �Լ��� ��������.
				0,						   // ����Ʈ ������ ���� �Ӽ�.
				(unsigned*)&dwThreadId[cntOfThread]   // ������ ID ������ ���� �ּҰ� ����.
			);

		// ������ ���� Ȯ��
		if (hThread[cntOfThread] == NULL)
		{
			_tprintf(_T("MAXIMUM THREAD SIZE: %d \n"), cntOfThread);
			break;
		}

		cntOfThread++;
	}

	for (DWORD i = 0; i < cntOfThread; i++)
	{
		CloseHandle(hThread[i]);
	}

	return 0;
}

//void main()
//{
//	vector<int> primes;
//
//	auto t0 = chrono::system_clock::now();
//
//	for (int i = 1; i < MaxCount; i++)
//	{
//		if (IsPrimeNumber(i))
//		{
//			primes.push_back(i);
//		}
//	}
//	PrintNumbers(primes);
//	auto t1 = chrono::system_clock::now();
//	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
//	cout << "Took" << duration << "milliseconds." << endl;
//	//150000 �õ� �� �� 7504milliseconds. 
//	//300000 �õ� �� �� 13710milliseconds.
//}