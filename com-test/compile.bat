rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
rem cl /I "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\VCWizards\AppWiz\ATL\ATLProject\templates\1033" test1.c
rem call that compiles okay

cl test1.cpp /Fetest1
cl test2.cpp /Fetest2

