all: get-files.exe test1.exe test2.exe test3.exe

test3.exe: test3.obj
    link -out:test3.exe test3.obj user32.lib

test2.exe: test2.obj sqlite3.obj util.obj
    link -out:test2.exe test2.obj sqlite3.obj util.obj user32.lib

test1.exe: test1.obj sqlite3.obj util.obj
    link -out:test1.exe test1.obj sqlite3.obj util.obj user32.lib

get-files.exe: get-files.obj
    link -out:get-files.exe get-files.obj user32.lib

get-files.obj: get-files.cpp
    cl /c /DUNICODE /D_CRT_SECURE_NO_WARNINGS get-files.cpp
	
sqlite3.obj: sqlite3.c
    cl /c /DUNICODE sqlite3.c

shell.obj: shell.c
	cl /c /DUNICODE shell.c

	test1.obj: test1.cpp
    cl /c /DUNICODE test1.cpp

test2.obj: test2.cpp
    cl /c /DUNICODE  test2.cpp

test3.obj: test3.cpp
    cl /c /DUNICODE test3.cpp

util.obj: util.cpp
    cl /c /DUNICODE util.cpp
