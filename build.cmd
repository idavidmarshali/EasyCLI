@echo off

cls
pushd "%~dp0"
echo Building EasyCLI object file..
gcc -c -o obj/EasyCLI.o -I. -O3 -Wall -Werror -Wpedantic -Wextra EasyCLI.c
echo [32mBuilding Completed![0m
echo Compiling DLL into build/libEasyCLI.dll
gcc -shared -o build/LibEasyCLI.dll -O3 -Wall -Werror -Wpedantic -Wextra obj/EasyCLI.o EasyCLI.h
echo Compiling STATIC into build/libEasyCLI.a
ar.exe rcs build/libEasyCLI.a obj/EasyCLI.o
echo [32mCompiling completed[0m
echo Deleting object file
cd obj/
del EasyCLI.o
echo [32mDONE![0m
pause

