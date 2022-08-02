#include <iostream>
#include <tchar.h>
#include <mutex>
//#include <thread>
//pthread와 차이점은둘의 가장 큰 차이점은 추상화라고 생각합니다. std::threadC++ 클래스 라이브러리입니다. 
//std::thread라이브러리에는 범위 잠금, 재귀 뮤텍스, future/promise 디자인 패턴 구현 등과 같은 많은 추상 기능이 포함되어 있습니다 .
// ㅇhttps://stackoverflow.com/questions/13134186/c11-stdthreads-vs-posix-threads 출저
#include <Windows.h>
#include <random>

#define THREAD 10
#define EVENT 10
using namespace std;

DWORD randTimeArr[10];
DWORD state[10];
//축제에서 조명을 터트린다고 설정
//

int num = 10;


unsigned int WINAPI thread1(LPVOID arg)
{
	UNREFERENCED_PARAMETER(arg);

	wcout << "조명(스레드)" << GetCurrentThreadId() << "를 키셔야합니다" << endl;
	
	
	WaitForSingleObject(
		hEvent, // event handle
		INFINITE);    // indefinite wait


	wcout << "조명(스레드)" << GetCurrentThreadId() << "는 준비되어있습니다." << endl;
	return 1;

}



int main()
{

	DWORD dwThreadIDs[THREAD];
	HANDLE hThreads[THREAD];
	HANDLE hEvent[EVENT];



	for (int i = 0; i < THREAD; i++)
	{
		hThreads[i] = (HANDLE)
			_beginthreadex
			(
				NULL,
				0,
				thread1,	
				NULL,
				CREATE_SUSPENDED,
				(unsigned*)&dwThreadIDs[i]
			);

		if(hThreads[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1;	
		}
	}

	for (int i = 0; i < EVENT; i++)
	{
		hEvent[i] = CreateEvent(NULL, TRUE, FALSE, TEXT("Already Light"));

		if (hEvent[i] == NULL)
		{
			_tprintf(_T("Event creation fault! \n"));
			return -1;
		}
	}



	for (int i = 0; i < THREAD; i++)
	{
		ResumeThread(hThreads[i]);
	}

	

	for (int i = 0; i < EVENT; i++)
	{
		SetEvent(hEvent[i]);

	}

	WaitForMultipleObjects(10, hThreads, TRUE, INFINITE);

	for (int i = 0; i < EVENT; i++)
	{
		CloseHandle(hEvent[i]);
	}
	


	wcout << "모든 조명(스레드)작동 완료" << endl;
	return 1;

}
