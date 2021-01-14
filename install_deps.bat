@echo off

pushd .
cd %1

rem build glfw
cmake .
rem TODO don't build tests and examples
msbuild GLFW.sln /p:Configuration=Release /p:Platform=x64
popd

rem copy lib file
mkdir lib
copy %1\src\Release\glfw3.lib lib

rem copy headers
mkdir include\GLFW
copy %1\include\GLFW include\GLFW

rem copy glad files
mkdir include\glad
copy %1\deps\glad include\glad
mkdir deps
copy %1\deps\glad_gl.c deps

rem copy other deps
copy %1\deps\linmath.h include