
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <assert.h>

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");

int p1()
{
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
		
	}

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

void main()
{
	p1();
}
