#define UNICODE
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <time.h>
#include <assert.h>
#include "util.h"

/* program to create a table and insert values */

/* function to pass as parameter to bind */
void bind_text_destructor(void *param)
{
	free(param);
}

int mainprocess(FILE *logfile)
{
	int res;
	sqlite3 *pDb; /* database handle*/
	sqlite3_stmt *pStmt; /* statement handle */
	char *utf8Statement, *utf8Filename;
	int utf8StatementLen, utf8FilenameLen;
	char *utf8databaseFileName;
	wchar_t *databaseFile = L"C:\\Users\\Father\\code-stuff\\sqlite-test\\test1-database.db";
    wchar_t *stmt;
	struct tm datetime;
	
	utf8databaseFileName = stringToUtf8(databaseFile, NULL);
	res = sqlite3_open(utf8databaseFileName, &pDb);
	if (res != SQLITE_OK)
	{
		MessageBox(NULL,L"Failed to open database",L"error",MB_OK);
		return -1;
	}
	free(utf8databaseFileName);
	stmt = L"CREATE TABLE IF NOT EXISTS FileTable ("\
					L"id INTEGER PRIMARY_KEY," \
					L"filename TEXT UNIQUE NOT NULL," \
					L"filesize INTEGER," \
					L"timestamp INTEGER)";
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
	if (res != SQLITE_DONE)
	{
		MessageBox(NULL,L"Failed to create table",L"error",MB_OK);
		return -1;
	}
	res = sqlite3_finalize(pStmt);
	assert(res == SQLITE_OK);
	stmt = L"INSERT INTO FileTable (filename, filesize, timestamp) VALUES (?, ?, ?)";
	utf8Statement = stringToUtf8(stmt, &utf8StatementLen);
	res = sqlite3_prepare_v2(pDb, utf8Statement, utf8StatementLen, 
	          &pStmt, NULL);
	if (res != SQLITE_OK)
	{
		MessageBox(NULL,L"Failed to prepare insert statement",L"error",MB_OK);
		return -1;
	}
	free(utf8Statement);
	/* bind filename */
	//utf8Filename = stringToUtf8(L"𠃯𠀀🞅.txt", &utf8FilenameLen);
	utf8Filename = stringToUtf8(L"test.txt", &utf8FilenameLen);
	//utf8Filename = stringToUtf8(L"א.txt", &utf8FilenameLen);
	res = sqlite3_bind_text(pStmt, 1, utf8Filename, utf8FilenameLen, bind_text_destructor);
	assert(res == SQLITE_OK);
	/* bind filesize */
	res = sqlite3_bind_int(pStmt, 2, 123888);
	assert(res == SQLITE_OK);
	/* bind timestamp */
	datetime.tm_hour = 12;
	datetime.tm_min = 38;
	datetime.tm_sec = 21;
	datetime.tm_isdst = 0;
	datetime.tm_mday = 13; // 13 december 2000
	datetime.tm_mon = 11; //december
	datetime.tm_year = 100;
	res = sqlite3_bind_int(pStmt, 3, mktime(&datetime));
	assert(res == SQLITE_OK);
	res = sqlite3_step(pStmt);
	if (res != SQLITE_DONE)
	{
		MessageBox(NULL,L"Failed to insert data to table",L"error",MB_OK);
		return -1;
	}
	res = sqlite3_finalize(pStmt);
	assert(res == SQLITE_OK);
	res = sqlite3_close_v2(pDb);
	assert(res == SQLITE_OK);
	return 0;
}

int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
{
    FILE *logfile;
	int result;
    logfile =  _wfopen(L"c:\\users\\father\\code-stuff\\sqlite-test\\test1-logfile.txt", L"w,ccs=UTF-8");
    if (!logfile)
    { 
        MessageBox(NULL,L"Failed to open logfile",L"error",MB_OK);
        return -1;
    }
	fwprintf(logfile,L"value of __STDC_ISO_10646__ is %d\n", __STDC_ISO_10646__);
    result = mainprocess(logfile);
	fwprintf(logfile,L"completed main process, result is %d\n", result);
    fclose(logfile);  
	MessageBox(NULL,L"Logfile closed",L"end",MB_OK);
	return 0;
}
