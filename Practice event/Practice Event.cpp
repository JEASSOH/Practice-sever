#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 4 
#define EVENTCOUNT 3

HANDLE ghWriteEvent;
HANDLE ghThreads[THREADCOUNT];
HANDLE ghEvent[EVENTCOUNT];

DWORD WINAPI ThreadProc(LPVOID);

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    // lpParam not used in this example.
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

    printf("Thread %d이 2번째 쓰레드를 키겠습니다.\n", GetCurrentThreadId());

    printf("2번째 쓰레드에게 신호주는중\n", GetCurrentThreadId());
    SetEvent(ghEvent[0]);
    return 1;
}
DWORD WINAPI ThreadProc2(LPVOID lpParam)
{
    // lpParam not used in this example.
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

    

    dwWaitResult = WaitForSingleObject(
        ghEvent[0], // event handle
        INFINITE);    // indefinite wait
    printf("신호를 받았습니다!.\n", GetCurrentThreadId());
   

    // Now that we are done reading the buffer, we could use another
    // event to signal that this thread is no longer reading. This
    // example simply uses the thread handle for synchronization (the
    // handle is signaled when the thread terminates.)
    printf("Thread %d이 3번째 쓰레드를 키겠습니다.\n", GetCurrentThreadId());

    printf("3번째 쓰레드에게 신호주는중\n", GetCurrentThreadId());
    SetEvent(ghEvent[1]);
    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}
DWORD WINAPI ThreadProc3(LPVOID lpParam)
{
    // lpParam not used in this example.
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

   

    dwWaitResult = WaitForSingleObject(
        ghEvent[1], // event handle
        INFINITE);    // indefinite wait
    printf("신호를 받았습니다!.\n", GetCurrentThreadId());
    

    // Now that we are done reading the buffer, we could use another
    // event to signal that this thread is no longer reading. This
    // example simply uses the thread handle for synchronization (the
    // handle is signaled when the thread terminates.)
     // handle is signaled when the thread terminates.)
    printf("Thread %d이 4번째 쓰레드를 키겠습니다.\n", GetCurrentThreadId());

    printf("4번째 쓰레드에게 신호주는중\n", GetCurrentThreadId());
    SetEvent(ghEvent[2]);
    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}
DWORD WINAPI ThreadProc4(LPVOID lpParam)
{
    // lpParam not used in this example.
    UNREFERENCED_PARAMETER(lpParam);
    DWORD dwWaitResult;

    dwWaitResult = WaitForSingleObject(
        ghEvent[2], // event handle
        INFINITE);    // indefinite wait
    printf("신호를 받았습니다!.\n", GetCurrentThreadId());
   

    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}
void CreateEventsAndThreads(void)
{
    int i;
    DWORD dwThreadID; //아이디를 담을 dword 변수 하나 생성

    // Create a manual-reset event object. The write thread sets this
    // object to the signaled state when it finishes writing to a 
    // shared buffer. 
    for (i = 0; i < THREADCOUNT; i++)
    {
        // TODO: More complex scenarios may require use of a parameter
        //   to the thread procedure, such as an event per thread to  
        //   be used for synchronization.
        ghEvent[i] = CreateEvent(
            NULL,               // default security attributes
            TRUE,               // manual-reset event
            FALSE,              // initial state is nonsignaled
            NULL  // object name
        );

        if (ghEvent[i] == NULL)
        {
            printf("CreateEvent failed (%d)\n", GetLastError());
            return;
        }
    }
    

    for (i = 0; i < THREADCOUNT; i++)
    {

        
        
        // TODO: More complex scenarios may require use of a parameter
        //   to the thread procedure, such as an event per thread to  
        //   be used for synchronization.
        if (i == 0)
        {
            ghThreads[i] = CreateThread(
                NULL,              // default security
                0,                 // default stack size
                ThreadProc,        // name of the thread function
                NULL,              // no thread parameters
                0,                 // default startup flags
                &dwThreadID);
        }
        else if (i == 1)
        {
            ghThreads[i] = CreateThread(
                NULL,              // default security
                0,                 // default stack size
                ThreadProc2,        // name of the thread function
                NULL,              // no thread parameters
                0,                 // default startup flags
                &dwThreadID);
        }
        else if (i == 2)
        {
            ghThreads[i] = CreateThread(
                NULL,              // default security
                0,                 // default stack size
                ThreadProc3,        // name of the thread function
                NULL,              // no thread parameters
                0,                 // default startup flags
                &dwThreadID);
        }
        else if (i == 3)
        {
            ghThreads[i] = CreateThread(
                NULL,              // default security
                0,                 // default stack size
                ThreadProc4,        // name of the thread function
                NULL,              // no thread parameters
                0,                 // default startup flags
                &dwThreadID);
        }
        if (ghThreads[i] == NULL)
        {
            printf("CreateThread failed (%d)\n", GetLastError());
            return;
        }
    }
    //4개의 쓰레드 생성
}



void CloseEvents()
{
    // Close all event handles (currently, only one global handle).

    CloseHandle(ghWriteEvent);
}

int main(void)
{
    DWORD dwWaitResult;

    // TODO: Create the shared buffer

    // Create events and THREADCOUNT threads to read from the buffer

    CreateEventsAndThreads();

    // At this point, the reader threads have started and are most
    // likely waiting for the global event to be signaled. However, 
    // it is safe to write to the buffer because the event is a 
    // manual-reset event.

    

    printf("Main thread waiting for threads to exit...\n");

    // The handle for each thread is signaled when the thread is
    // terminated.
    dwWaitResult = WaitForMultipleObjects(
        THREADCOUNT,   // number of handles in array
        ghThreads,     // array of thread handles
        TRUE,          // wait until all are signaled
        INFINITE);

    switch (dwWaitResult)
    {
        // All thread objects were signaled
    case WAIT_OBJECT_0:
        printf("All threads ended, cleaning up for application exit...\n");
        break;

        // An error occurred
    default:
        printf("WaitForMultipleObjects failed (%d)\n", GetLastError());
        return 1;
    }

    // Close the events to clean up

    CloseEvents();

    return 0;
}

