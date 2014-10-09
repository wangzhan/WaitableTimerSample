// WaitableTimerSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "iostream"

void test1()
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	// 5s
	liDueTime.QuadPart = -50000000LL;

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (NULL == hTimer)
	{
		printf("CreateWaitableTimer failed (%d)\n", GetLastError());
		return;
	}

	printf("Waiting for 5 seconds...\n");

	// Set a timer to wait for 5 seconds.
	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		printf("SetWaitableTimer failed (%d)\n", GetLastError());
	}

	// Wait for the timer.

	if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		printf("WaitForSingleObject failed (%d)\n", GetLastError());
	else
		printf("Timer was signaled.\n");

	CloseHandle(hTimer);
}

static int g_iCount = 0;

VOID CALLBACK TimerAPCProc(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	++g_iCount;

	std::cout << "Timer is signaled: " << g_iCount << std::endl;
}

void test2()
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	// 5s
	liDueTime.QuadPart = -50000000LL;

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, FALSE, L"Test Timer");
	if (NULL == hTimer)
	{
		printf("CreateWaitableTimer failed (%d)\n", GetLastError());
		return;
	}

	printf("Waiting for 5 seconds...\n");

	// 2s
	LONG lPeriod = 2000;

	// Set a timer to wait for 5 seconds.
	if (!SetWaitableTimer(hTimer, &liDueTime, lPeriod, TimerAPCProc, NULL, 0))
	{
		printf("SetWaitableTimer failed (%d)\n", GetLastError());
	}

	// Wait for the timer.
	while (g_iCount < 10)
	{
		SleepEx(INFINITE, TRUE);
	}

	CloseHandle(hTimer);
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Please enter a flag number:" << std::endl;
	std::cout << "1: One loop caller" << std::endl;
	std::cout << "2: Period loop caller" << std::endl;
	std::cout << "others means exit" << std::endl;

	int iFlag = 0;
	if (std::cin >> iFlag)
	{
		if (iFlag == 1)
		{
			test1();
		}
		else if (iFlag == 2)
		{
			test2();
		}
	}

	std::cout << "Press any key to continue ..." << std::endl;
	std::cin.get();

	return 0;
}

