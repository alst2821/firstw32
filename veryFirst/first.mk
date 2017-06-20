#sample makefile
#!include <win32.mak>

all: first.exe

.cpp.obj:
	cl /c /Wall  $*.cpp

first.exe: first.obj
	link -out:first.exe first.obj User32.Lib

#.cpp.obj:
#  $(cc) $(cdebug) $(cflags) $(cvars) $*.cpp

#first.exe: first.obj
#  $(link) $(ldebug) $(conflags) -out:first.exe first.obj
