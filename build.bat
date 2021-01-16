@echo off
if not exist build mkdir build

set CFLAGS=/nologo /I"include" /Fo"build\\" /std:c11 /MD /O2 /Zc:inline /W4 /WX /wd5105
set LFLAGS=/nologo /ENTRY:"mainCRTStartup"
rem /SUBSYSTEM:WINDOWS 
set LIBS=lib\glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib
set SOURCES=src\main.c src\pga2d.c deps\*

cl %CFLAGS% /c %SOURCES%
link %LFLAGS% %LIBS% build\*.obj /OUT:build\main.exe