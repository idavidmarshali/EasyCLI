@echo off

cls

pushd "%~dp0"
echo "Building EasyCLIShared object file.."
gcc -c -fPIC -o obj/EasyCLIShared.o -I. -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "[32mBuilding Completed![0m"
echo "Compiling DLL into build/libEasyCLI.dll"
gcc -shared -fPIC -o build/LibEasyCLI.dll -O3 -Wall -Werror -Wpedantic -Wextra obj/EasyCLIShared.o
echo "Building EasyCLIStatic object file.."
gcc -c -o obj/EasyCLIStatic.o -I. -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "Compiling STATIC into build/libEasyCLI.a"
ar rcs build/libEasyCLI.a obj/EasyCLIStatic.o
echo "[32mCompiling completed[0m"
echo "Deleting object files"
cd obj/
del EasyCLI.o
del EasyCLIStatic.o EasyCLIShared.o
echo [32mDONE![0m
pause

