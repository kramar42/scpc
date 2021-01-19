@echo off

if "%~1" == "" echo "Specify GLFW location" && exit /b

rem delete old files
del deps include /q

pushd .
cd %1

rem build glfw
cmake .
rem TODO don't build tests and examples
msbuild GLFW.sln /p:Configuration=Release /p:Platform=x64
popd

rem copy lib file
if not exist lib mkdir lib
copy %1\src\Release\glfw3.lib lib

rem copy headers
if not exist include\GLFW mkdir include\GLFW
copy %1\include\GLFW include\GLFW

rem requires package to be installed: pip3 install glad
rem generate glad files
python3 -m glad --profile core --out-path %tmp%\glad --api gl=3.3 --generator c --spec gl --extensions=

rem copy headers
if not exist include\glad mkdir include\glad
copy %tmp%\glad\include\glad include\glad
if not exist include\KHR mkdir include\KHR
copy %tmp%\glad\include\KHR include\KHR

rem copy source
if not exist deps mkdir deps
copy %tmp%\glad\src deps

rem download other dependencies
curl https://raw.githubusercontent.com/nothings/stb/master/stb_ds.h -o include\std_ds.h
