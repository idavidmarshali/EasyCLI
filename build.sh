#!/bin/bash

BASEDIR=$(dirname "$0")
cd "$BASEDIR"
echo "Compiling Shared library into build/libEasyCLI.so .."
gcc -shared -fPIC -o build/libEasyCLI.so -I ./include -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "[32mCompiling Completed![0m"
echo "Compiling Static library into build/libEasyCLI.a .."
gcc -c -o obj/EasyCLIStatic.o -I ./include  -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
ar -rcs build/libEasyCLI.a obj/EasyCLIStatic.o
ranlib build/libEasyCLI.a
echo "[32mCompiling completed[0m"
echo "Deleting object files"
rm obj/EasyCLIStatic.o
echo "[32mDONE![0m"
read -p "Press any key to continue ..."

