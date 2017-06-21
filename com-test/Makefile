all: test1.exe test2.exe

.cpp.obj:
	cl /c  $*.cpp

test1.exe: test1.obj
  link -out:test1.exe test1.obj

test2.exe: test2.obj
  link -out:test2.exe test2.obj

