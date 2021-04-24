// https://docs.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <assert.h>
#include "event.h"

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");

int p1()
{
	GraphApi::Event evt(true, "my_g_event");
	HANDLE hMapFile;
	LPCTSTR pBuf;

	printf("create\n");

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}
	pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}

	printf("input command, q for quit\n");
	while(1)
	{
		char s[100];
		fgets(s, 100, stdin);

		CopyMemory((PVOID)pBuf, s, 100);
		assert(evt.set());
	}

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

int p2()
{
	GraphApi::Event evt(true, "my_g_event");
	HANDLE hMapFile;
	LPCTSTR pBuf;

	printf("open\n");
	while(1)
	{
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			szName);               // name of mapping object

		if (hMapFile == NULL)
		{
			_tprintf(TEXT("Could not open file mapping object (%d).\n"),
				GetLastError());
			Sleep(1000);
		}
		else
		{
			break;
		}
	}

	pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}

	while(1)
	{
		assert(GraphApi::Event::Ok==evt.wait());
		assert(evt.reset());
		printf("received %s\n", pBuf);
	}
	

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

int main(int argc, char *argv[])
{
	for (int i=0; i<argc; i++)
	{
		printf(">>arg[%d]=%s\n", i, argv[i]);
	}

	if (argc==1)
	{
		char s[_MAX_PATH];
		sprintf(s, "start %s p2", argv[0]);
		system(s);


		system("pause");
		p1();
	}
	else
	{
		p2();
	}
}
