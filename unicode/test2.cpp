#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <locale.h>
#include "sqlite3.h"
#include "util.h"

void report_row(const wchar_t *name, const char *size, const time_t time, FILE * logfile)
{
	int wname_len, wsize_len;
	wchar_t *wname, *wsize;
	struct tm *timedate;
	//wname = utf8ToString(name, &wname_len);
	wname_len = wcslen(name);
	wsize = utf8ToString(size, &wsize_len);
	timedate = localtime(&time);
	fwprintf(logfile, L"%s (%d) %8s (%d) %s\n", name, wname_len, 
	                  wsize, wsize_len, _wasctime(timedate));
}

/*  return utf8 string from column 
 */
const char *get_string(sqlite3_stmt *pStmt, int col, int *n_bytes)
{
	const char *utf8_string;
	int utf8_len;
	utf8_string  = reinterpret_cast<const char *>(sqlite3_column_text(pStmt, col));
	utf8_len = sqlite3_column_bytes(pStmt,col);
	if (n_bytes != NULL)
		*n_bytes = utf8_len;
	return utf8_string;
}

/*  return utf16 string from column 
 */
const wchar_t *get_wstring(sqlite3_stmt *pStmt, int col, int *n_bytes)
{
	const wchar_t *utf16_string;
	int utf16_len; //bytes
	utf16_string  = static_cast<const wchar_t *>(sqlite3_column_text16(pStmt, col));
	utf16_len = sqlite3_column_bytes16(pStmt,col);
	if (n_bytes != NULL)
		*n_bytes = utf16_len;
	return utf16_string;
}


/* program to open database and read values */
int mainprocess(FILE *logfile)
{
	int res;
	sqlite3 *pDb; /* database handle*/
	sqlite3_stmt *pStmt; /* statement handle */
	char *utf8Statement;
	int utf8StatementLen;
    const wchar_t *utf16Filename;
	int utf16FilenameLen;
	char *utf8databaseFileName;
	wchar_t *databaseFile = L"C:\\Users\\Father\\code-stuff\\sqlite-test\\test1-database.db";
    wchar_t *stmt;
	int column_count;
	int type0;
	int type1;
	int type2;

	time_t filetime;
	const char *filesize;
	
	utf8databaseFileName = stringToUtf8(databaseFile, NULL);
	res = sqlite3_open(utf8databaseFileName, &pDb);
	if (res != SQLITE_OK)
	{
		MessageBox(NULL,L"Failed to open database",L"error",MB_OK);
		return -1;
	}
	free(utf8databaseFileName);
	stmt = L"SELECT filename, filesize, timestamp FROM FileTable";
	utf8Statement = stringToUtf8(stmt, &utf8StatementLen);
	res = sqlite3_prepare_v2(pDb, utf8Statement, utf8StatementLen, 
	          &pStmt, NULL);
	if (res != SQLITE_OK)
	{
		MessageBox(NULL,L"Failed to prepare statement",L"error",MB_OK);
		return -1;
	}
	free(utf8Statement);
	res = sqlite3_step(pStmt);
	switch (res)
	{
		case SQLITE_BUSY:
			MessageBox(NULL,L"sqlite returned SQLITE_BUSY",L"error",MB_OK);
			return -1;
		case SQLITE_ERROR:
			MessageBox(NULL,L"sqlite returned SQLITE_ERROR",L"error",MB_OK);
			return -1;
		case SQLITE_MISUSE:
			MessageBox(NULL,L"sqlite returned SQLITE_MISUSE",L"error",MB_OK);
			return -1;
		case SQLITE_ROW:
		case SQLITE_DONE:
			break;
		default:
			MessageBox(NULL,L"sqlite returned unexpected value",L"error",MB_OK);
			return -1;
	}
	while (res == SQLITE_ROW)
	{
		/* https://sqlite.org/c3ref/column_blob.html */
		column_count = sqlite3_column_count(pStmt);
		assert(column_count == 3);
		type0 = sqlite3_column_type(pStmt, 0);
		type1 = sqlite3_column_type(pStmt, 1);
		type2 = sqlite3_column_type(pStmt, 2);
		assert(type0 == SQLITE_TEXT);
		assert(type1 == SQLITE_INTEGER);
		assert(type2 == SQLITE_INTEGER);
		utf16Filename = get_wstring(pStmt, 0, &utf16FilenameLen);
		filesize = get_string(pStmt, 1, NULL);
		filetime = sqlite3_column_int(pStmt, 2);
		report_row(utf16Filename, filesize, filetime, logfile);
		res = sqlite3_step(pStmt);
	}
	res = sqlite3_finalize(pStmt);
	assert(res == SQLITE_OK);
	res = sqlite3_close_v2(pDb);
	assert(res == SQLITE_OK);
	return 0;
}

int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
{
	setlocale(LC_ALL, "jpn");
    FILE *logfile;
	int result;
    logfile =  _wfopen(L"c:\\users\\father\\code-stuff\\sqlite-test\\test2-logfile.txt", L"w,ccs=UTF-8");
    if (!logfile)
    { 
        MessageBox(NULL,L"Failed to open logfile",L"error",MB_OK);
        return -1;
    }
	fwprintf(logfile,L"logfile start %ls \n", L"(𠃯𠀀🞅)");
	fwprintf(logfile,L"-- %ls --\n", L"א");
    result = mainprocess(logfile);
	fwprintf(logfile,L"completed main process, result is %d\n", result);
    fclose(logfile);  
	MessageBox(NULL,L"Logfile closed",L"end",MB_OK);
	return 0;
}
