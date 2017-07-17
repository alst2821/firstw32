#include <windows.h>
#include <stdio.h>
#include <assert.h>

/* program to count the number of utf-8 characters in a file*/

typedef enum utf8_len { 
	one_byte = 0, 
	two_bytes, 
	three_bytes, 
	four_bytes,
	unknown
}  utf8_len_t;

int count_chars(FILE *logfile)
{
	int c;
	int count = 0;
	int histogram[5];
	FILE * file1  = 
	    _wfopen(L"c:\\users\\father\\code-stuff\\sqlite-test\\test3-input.txt",
         L"rb");
	utf8_len_t char_len;
	histogram[one_byte] = 0;
	histogram[two_bytes] = 0;
	histogram[three_bytes] = 0;
	histogram[four_bytes] = 0;
	histogram[unknown] = 0;
	c = fgetc(file1);
	while (c != EOF)
	{
		char_len = one_byte;
		if ( c & 0x80 == 0) {
			char_len = one_byte;
		}
		else if ( ( (c & 0xc0) == 0xc0) 
			  && (c&0x20 == 0))
		{
			char_len = two_bytes;
		} 
		else if ( ( (c&0xe0) == 0xe0)
			&& (c&0x10 == 0x0))
		{
			char_len = three_bytes;
		}
		else if ( ( (c&0xf0) == 0xf0)
			&& ((c&0x08) == 0x0) )
		{
			char_len = four_bytes;
		}
		else 
		{
			assert(false);
			char_len = unknown;
		}
		histogram[(int)char_len] ++;
		//complete
		switch (char_len)
		{
			case four_bytes:
				c = fgetc(file1);
				assert( c != EOF );
				assert( c & 0xc0 == 0x80 );
				//fall through
			case three_bytes:
				c = fgetc(file1);
				assert( c != EOF );
				assert( c & 0xc0 == 0x80 );
				//fall through
			case two_bytes:
				c = fgetc(file1);
				assert( c != EOF );
				assert( c & 0xc0 == 0x80 );
				//fall through
			case one_byte:
				break;
		}
		count++;
		c = fgetc(file1);
	}
	fclose(file1);
	fprintf(logfile,"results\n");
	fprintf(logfile,"character count is %d\n", count);
	fprintf(logfile,"histogram is\n");
	for (int i = 0; i<5;++i)
	{
		fprintf(logfile,"entry %d count is %d\n", i, histogram[i]);
	}
	fprintf(logfile,"end of histogram\n");
	return 0;
}


int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
{
    FILE *logfile;
	int result;
    logfile =  _wfopen(L"c:\\users\\father\\code-stuff\\sqlite-test\\test3-logfile.txt", L"w,ccs=UTF-8");
    if (!logfile)
    { 
        MessageBox(NULL,L"Failed to open logfile",L"error",MB_OK);
        return -1;
    }
	result = count_chars(logfile);
	fwprintf(logfile,L"completed main process, result is %d\n", result);
    fclose(logfile);  
	MessageBox(NULL,L"Logfile closed",L"end",MB_OK);
	return 0;
}
