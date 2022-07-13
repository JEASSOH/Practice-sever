#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

const int MaxCount = 300000;
const int ThreadCount = 4;

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

int main()
{
    // 각 스레드는 여기서 값을 꺼내온다.
    int num = 1;

    recursive_mutex num_mutex;
    recursive_mutex prime_mutex;
    
    vector<int> primes; // 벡터 int형 primes 변수 생성
     

    auto t0 = chrono::system_clock::now(); // 시간체크 시작

    // 작동할 워커 스레드
    vector<shared_ptr<thread> > threads; 

    for (int i = 0; i < ThreadCount; i++)
    {
        shared_ptr<thread> thread1(new thread([&]() {
            // 각 스레드의 메인 함수.
            // 값을 가져올 수 있으면 루프를 돈다.
            while (true)
            {
                int n;
                {
                    lock_guard<recursive_mutex> num_lock(num_mutex); //lock 걸림
                    n = num;
                    num++;
                }//num_mutex lcok 해제

                if (n >= MaxCount)
                    break;

                if (IsPrimeNumber(n))
                {
                    lock_guard<recursive_mutex> prime_lock(prime_mutex);// lock걸림
                    primes.push_back(n);
                }//lock해제 (unlock)
            }
            }));
        // 스레드 객체를 일단 갖고 있는다.
        threads.push_back(thread1);
    }

    // 모든 스레드가 일을 마칠 때까지 기다린다.
    for (auto thread : threads)
    {
        thread->join();
    }   
    // 끝

    auto t1 = chrono::system_clock::now(); //시간체크 끝

    auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cout << "Took " << duration << " milliseconds." << endl;

    //    PrintNumbers(primes);
    ////150000 시도 시 약 392milliseconds. 
    ////300000 시도 시 약 1491milliseconds. 
    return 0;
}