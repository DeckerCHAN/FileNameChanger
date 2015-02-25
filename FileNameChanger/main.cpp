#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include<iostream>
#include<string>
#pragma comment(lib, "User32.lib")
using namespace std;

bool hasEnding(wstring const &fullString, wstring const &ending);
int _tmain(int argc, TCHAR *argv[])
{

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// If the directory is not specified as a command-line argument,
	// print usage.

	if (argc != 2)
	{
		_tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
		return (-1);
	}

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(argv[1], MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, argv[1]);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		_tprintf(TEXT("CantFindFirstFile"));
		return dwError;
	}

	// List all the files in the directory with some info about them.
	int counter = 1;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			wstring strA=argv[1];
			strA.append(L"\\");
			wstring strB=ffd.cFileName;
			if (hasEnding(strB, L"jpg"))
			{
			
				wstring fullFileName = L"";
				fullFileName.append(strA);
				fullFileName.append(strB);
				wstring arentFileName = L"";
				arentFileName.append(strA);
				arentFileName.append(to_wstring(counter));
				arentFileName.append(L".jpg");
				_tprintf(TEXT("I feel this is a jpg file, I would rename it from %s to %s\n"), fullFileName.c_str(), arentFileName.c_str());
				_wrename(fullFileName.c_str(), arentFileName.c_str());
				counter++;
			}

		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		_tprintf(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return dwError;
}

bool hasEnding(wstring const &fullString, wstring const &ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else
	{
		return false;
	}

}