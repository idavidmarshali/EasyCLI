!bin/bash
clear

BASEDIR=$(dirname "$0")
cd "$BASEDIR"
echo "Building EasyCLI object file.."
gcc -c -o obj/EasyCLI.o -I. -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "[32mBuilding Completed![0m"
echo "Compiling DLL into build/libEasyCLI.dll"
gcc -shared -fPIC -o build/LibEasyCLI.dll -O3 -Wall -Werror -Wpedantic -Wextra obj/EasyCLI.o include/EasyCLI.h
echo "Compiling STATIC into build/libEasyCLI.a"
ar.exe rcs build/libEasyCLI.a obj/EasyCLI.o
echo "[32mCompiling completed[0m"
echo "Deleting object file"
rm obj/EasyCLI.o
echo "[32mDONE![0m"
read -p "Press any key to continue ..."

