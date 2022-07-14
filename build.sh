!bin/bash

clear

BASEDIR=$(dirname "$0")
cd "$BASEDIR"
echo "Building EasyCLIShared object file.."
gcc -c -fPIC -o obj/EasyCLIShared.o -I ./include -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "[32mBuilding Completed![0m"
echo "Compiling SharedLib into build/libEasyCLI.so"
gcc -shared -fPIC -o build/LibEasyCLI.so -O3 -Wall -Werror -Wpedantic -Wextra obj/EasyCLIShared.o
echo "Building EasyCLIStatic object file.."
gcc -c -o obj/EasyCLIStatic.o -I ./include  -O3 -Wall -Werror -Wpedantic -Wextra src/EasyCLI.c
echo "Compiling STATIC into build/libEasyCLI.a"
ar rcs build/libEasyCLI.a obj/EasyCLIStatic.o
echo "[32mCompiling completed[0m"
echo "Deleting object files"
rm obj/EasyCLIShared.o obj/EasyCLIStatic.o
echo "[32mDONE![0m"
read -p "Press any key to continue ..."

