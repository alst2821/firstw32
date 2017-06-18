.cpp.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.cpp

first.exe: first.obj
  $(link) $(ldebug) $(conflags) -out:first.exe first.obj
