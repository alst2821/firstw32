#include <windows.h>
#include <stdio.h>
#define ROOT TEXT("c:\\Users\\Father\\code-stuff\\sqlite-test\\*")
#define LOGFILE TEXT("c:\\users\\father\\code-stuff\\sqlite-test\\logeUTF16.txt")

//#define _CRT_SECURE_NO_WARNINGS 1

int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
{
	DWORD64 filesize;
    FILE *logfile;
    WIN32_FIND_DATA fd;
	BOOL okay;
    HANDLE hFind;
	okay = 1;
    logfile =  _wfopen(LOGFILE, L"w,ccs=UTF-16LE");
    if (!logfile)
    { 
        MessageBox(NULL,L"Failed to open logfile",L"error",MB_OK);
        return -1;
    }
    fwprintf(logfile, L"hello logfile!\n");

    hFind = ::FindFirstFile(ROOT, &fd);
    if(hFind==INVALID_HANDLE_VALUE){
		fwprintf(logfile, L"invalid file handle\n");
		okay = 0;
    }
    else {
	    fwprintf(logfile, L"valid file handle\n");
    }
	while (okay) {
		filesize = fd.nFileSizeLow | (((DWORD64)fd.nFileSizeHigh) << 32);
		fwprintf(logfile, L"%s %lld\n", fd.cFileName, filesize);
		okay = FindNextFile(hFind, &fd);
	}
    fwprintf(logfile, L"bye bye logfile!\n");
    fclose(logfile);  
	MessageBox(NULL,L"Logfile closed",L"error",MB_OK);
  return 0;
}
/*

https://msdn.microsoft.com/en-us/library/windows/desktop/aa365258(v=vs.85).aspx

  //directory handle with CreateFile FILE_FLAG_BACKUP_SEMANTICS semantics flag


Listing the files follows an example at this location
https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx


FindFirstFile
FindNextFile
FindClose
WIN32_FIND_DATA
dwFileAttributes
FILE_ATTRIBUTE_DIRECTORY
FILE_ATTRIBUTE_REPARSE_POINT

FindFirstFile function
https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
Windows Dev Center Location breadcrums
Develop >> Desktop Technologies >> Data Access and Storage >> Local File Systems >>
File Management >> File Management Reference >> File Management Functions


WIN32_FIND_DATA
https://msdn.microsoft.com/en-us/library/windows/desktop/aa365740(v=vs.85).aspx
  DWORD    dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
  DWORD    dwReserved0;
  DWORD    dwReserved1;
  TCHAR    cFileName[MAX_PATH];
  TCHAR    cAlternateFileName[14];
  
Notes on 7 July 2017
I have to create the database

proposed so far:
name (full path) TEXT
file size  INT
creation time TIMEDATE

CREATE TABLE IF NOT EXISTS FileTable (
 id INTEGER PRIMARY_KEY,
 filename TEXT UNIQUE NOT NULL,
 filesize INTEGER,
 timestamp INTEGER
)
INSERT INTO FileTable 
 ( filename, filesize, timestamp)
 VALUES (?, ?, ?)
*/