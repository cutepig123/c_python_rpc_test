#include <iostream>
#include "TimXmlRpc.h"
#include <Windows.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <assert.h>

#define BUF_SIZE 256
char szName[]=("Global\\MyFileMappingObject");
// main.cpp:   includes both a little unit test and a sample client program.


static void BasicTest()
{
	// Replace this URL with your own URL:
	XmlRpcClient Connection("http://web.edval.com.au/rpc");
	Connection.setIgnoreCertificateAuthority();
	//Connection.setBasicAuth_Callback(PopUpAPrettyDialog);
	Connection.setBasicAuth_UsernameAndPassword("foo", "goo");

	//  Call:  arumate.getKilowatts(string, integer)   :
	XmlRpcValue args, result;
	args[0] = "test";
	args[1] = 1;

	// 
	double g = 3.14159;
	XmlRpcValue binary(&g, sizeof(g));
	args[2] = binary;
	XmlRpcValue::BinaryData bdata = binary;

	// Replace this function name with your own:
	if (! Connection.execute("getKilowatts", args, result)) {
		std::cout << Connection.getError();
	}
	else if (result.getType() == XmlRpcValue::TypeString)
		std::cout << result.GetStdString();
	else std::cout << "Success\n";
}

#define VERIFY(x)	if(!(x)) printf("error " #x "\n");

static void BasicPythonTest()
{
	// Replace this URL with your own URL:
	XmlRpcClient Connection("http://localhost:8000/");
	//Connection.setIgnoreCertificateAuthority();
	//Connection.setBasicAuth_Callback(PopUpAPrettyDialog);
	//Connection.setBasicAuth_UsernameAndPassword("foo", "goo");

	//  Call:  arumate.getKilowatts(string, integer)   :
	XmlRpcValue args, result;
	args = 3;
	
	size_t n = 1e3;
	DWORD t1 = GetTickCount();
	for(size_t i=0; i<n; i++)
	{
		// Replace this function name with your own:
		if (!Connection.execute("is_even", args, result)) {
			std::cout << i << ":" << Connection.getError() << std::endl;
		}
		VERIFY (result.getType() == XmlRpcValue::TypeBoolean);
		VERIFY(false==(bool)result);
	}
	DWORD t2 = GetTickCount();
	double timePercallInMs = (double)(t2-t1)/n;
	printf("is_even timePercallInMs %f\n", timePercallInMs);
}

class SM
{
public:
	HANDLE hMapFile;
	LPCTSTR pBuf;

	int create_sm()
	{
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

		*(char*)pBuf = 100;
	}

	void close_sm()
	{
		UnmapViewOfFile(pBuf);

		CloseHandle(hMapFile);
	}
};

static void BasicPythonShareMemTest()
{
	SM sm;
	sm.create_sm();

	// Replace this URL with your own URL:
	XmlRpcClient Connection("http://localhost:8000/");
	//Connection.setIgnoreCertificateAuthority();
	//Connection.setBasicAuth_Callback(PopUpAPrettyDialog);
	//Connection.setBasicAuth_UsernameAndPassword("foo", "goo");

	//  Call:  arumate.getKilowatts(string, integer)   :
	XmlRpcValue args, result;
	args = szName;

	size_t n = 1e3;
	DWORD t1 = GetTickCount();
	for(size_t i=0; i<n; i++)
	{
		// Replace this function name with your own:
		if (!Connection.execute("get_share_mem_1st_byte", args, result)) {
			std::cout << i << ":" << Connection.getError() << std::endl;
		}
		VERIFY (result.getType() == XmlRpcValue::TypeInt);
		VERIFY(100==(int)result);
	}
	DWORD t2 = GetTickCount();
	double timePercallInMs = (double)(t2-t1)/n;
	printf("BasicPythonShareMemTest timePercallInMs %f\n", timePercallInMs);

	sm.close_sm();
}

static void AdvancedTest()
{
	XmlRpcValue args, result;

	// Passing datums:
	args[0] = "a string";
	args[1] = 1;
	args[2] = true;
	args[3] = 3.14159;
	struct tm timeNow;
	args[4] = XmlRpcValue(&timeNow);

	// Passing an array:
	XmlRpcValue array;
	array[0] = 4;
	array[1] = 5;
	array[2] = 6;
	args[5] = array;
	// Note: if there's a chance that the array contains zero elements,
	// you'll need to call:
	//      array.initAsArray();
	// ...because otherwise the type will never get set to "TypeArray" and
	// the value will be a "TypeInvalid".

	// Passing a struct:
	XmlRpcValue record;
	record["SOURCE"] = "a";
	record["DESTINATION"] = "b";
	record["LENGTH"] = 5;
	args[6] = record;
	// We don't support zero-size struct's...Surely no-one needs these?

	// Make the call:
	XmlRpcClient Connection("https://61.95.191.232:9600/arumate/rpc/xmlRpcServer.php");
	Connection.setIgnoreCertificateAuthority();
	if (! Connection.execute("arumate.getMegawatts", args, result)) {
		std::cout << Connection.getError();
		return;
	}

	// Pull the data out:
	if (result.getType() != XmlRpcValue::TypeStruct) {
		std::cout << "I was expecting a struct.";
		return;
	}
	int i = result["n"];
	std::string s = result["name"];
	array = result["A"];
	for (int i=0; i < array.size(); i++)
		std::cout << (int)array[i] << "\n";
	record = result["subStruct"];
	std::cout << (std::string)record["foo"] << "\n";
}



//---------------------------- main(): -------------------------

int main(int argc, char* argv[])
{
	//BasicTest();
	//AdvancedTest();
	BasicPythonTest();
	BasicPythonShareMemTest();
	return 0;
}

