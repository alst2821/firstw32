/* util.cpp */
#include <windows.h>

wchar_t * utf8ToString(const char * utf8String, int * nchars)
{
	wchar_t *utf16String;
	int utf16len;
	int len1;
	UINT codepage = CP_UTF8;
	DWORD flags = MB_ERR_INVALID_CHARS;
	int stringlen = -1; // all characters until null terminator
	/* work out length */
	utf16len = MultiByteToWideChar(codepage, flags, utf8String, stringlen, NULL, 0);
	utf16String = static_cast<wchar_t *>(malloc(utf16len));
	/* transform*/
	len1 = MultiByteToWideChar(codepage, flags, utf8String, stringlen, utf16String, utf16len);
	if (nchars != NULL)
		*nchars = len1;
	return utf16String;
}

char * stringToUtf8(wchar_t *str, int *nByte)
{
	int utf8len;
	int len1;
	char *utf8string;
	UINT codepage = CP_UTF8;
	DWORD flags = WC_ERR_INVALID_CHARS;
	int stringlen = -1; // all characters until null terminator
	utf8len = WideCharToMultiByte(codepage,	flags, str,	stringlen, NULL, 0, NULL, NULL);
	if (utf8len == 0)
	{
		MessageBox(NULL,L"Error in stringToUtf8",L"error",MB_OK);
		exit(1);
	}
	utf8string = static_cast<char *>(malloc(utf8len));
	if (utf8len == 0)
	{
		MessageBox(NULL,L"Error in stringToUtf8 (allocation)",L"error",MB_OK);
		exit(1);
	}
	len1 = WideCharToMultiByte(codepage, flags, str, stringlen, utf8string, utf8len, NULL, NULL);
	if (nByte != NULL)
		*nByte = len1;
	return utf8string;
}
