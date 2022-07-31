#include <iostream>
#include <tchar.h>
#include <mutex>
//#include <thread>
//pthread와 차이점은둘의 가장 큰 차이점은 추상화라고 생각합니다. std::threadC++ 클래스 라이브러리입니다. 
//std::thread라이브러리에는 범위 잠금, 재귀 뮤텍스, future/promise 디자인 패턴 구현 등과 같은 많은 추상 기능이 포함되어 있습니다 .
// ㅇhttps://stackoverflow.com/questions/13134186/c11-stdthreads-vs-posix-threads 출저
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>
#include <random>

std::random_device rd;

// random_device 를 통해 난수 생성 엔진을 초기화 한다.
std::mt19937 gen(rd());

std::normal_distribution<> dis(30, 10);

using namespace std; //std

sem_t semaphore;

#define THREAD 10

DWORD randTimeArr[10];

//세마포어 예제 화장실 두개와 이용자 10명
//화장실은 랜덤시간(20~50)초간 사용한다 가정

int num = 10;


unsigned int WINAPI thread1(LPVOID arg)
{
	int usingTime = (int)arg;

	if (num != 0)
	{
		num--;

		sem_wait(&semaphore);
		wcout << "스레드" << GetCurrentThreadId() << "은 앞으로" <<usingTime<<"초간 화장실을 이용할 예정입니다" << endl;
		Sleep(usingTime*1000);
		wcout << "스레드" << GetCurrentThreadId() << "은 화장실에서 나왔습니다." << endl;
		num++;

		sem_post(&semaphore);

	}

	else if (num == 0)
	{
		wcout << "스레드" << GetCurrentThreadId() << "은 화장실을 기다리고 있습니다." << endl;

		while (num == 0) {}

		wcout << "스레드" << GetCurrentThreadId() << "은 화장실 이용이 끝났습니다." << endl;
		sem_post(&semaphore);
		num++;

	}
	return NULL;

}


int main()
{

	DWORD dwThreadIDs[THREAD];
	HANDLE hThreads[THREAD];
	sem_init(&semaphore, 0, 2);

	for (int i = 0; i < THREAD; i++)
	{
		randTimeArr[i] = (DWORD)(dis(gen));

	}


	for (int i = 0; i < THREAD; i++)
	{
		hThreads[i] = (HANDLE)
			_beginthreadex(
				NULL,
				0,
				thread1,
				(void*)randTimeArr[i],
				CREATE_SUSPENDED,
				(unsigned*)&dwThreadIDs[i]
			);

		if (hThreads[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1;
		}
	}

	
	

	for (int i = 0; i < THREAD; i++)
	{
		ResumeThread(hThreads[i]);
	}

	WaitForMultipleObjects(10, hThreads, TRUE, INFINITE);

	sem_destroy(&semaphore);

}
