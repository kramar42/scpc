@echo off
del build /q

set CFLAGS=/nologo /I"include" /Fo"build\\" /std:c11 /MD /O2 /Zc:inline /W4 /wd5105 /wd4456 /wd4221 /WX
set LFLAGS=/nologo /ENTRY:"mainCRTStartup" /NODEFAULTLIB:LIBCMT
rem /SUBSYSTEM:WINDOWS
set LIBS=lib\glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib
set SOURCES=src\*.c deps\*.c

cl %CFLAGS% /c %SOURCES%
link %LFLAGS% %LIBS% build\*.obj /OUT:build\main.exe