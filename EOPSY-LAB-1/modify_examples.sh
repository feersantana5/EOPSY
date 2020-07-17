#!/bin/sh
#EOPSY lab 1
#Fernando Santana Falcon group 102

echo "The goal of modify.sh is to change the names of the files converting
uppercase letters to lowercase and vice versa. modify_example.sh tests the main
script."

 echo "
Test 1 executing without args:"
 ./modify.sh

echo "
Test 2 shows help:"
./modify.sh -h

echo "
Test 3 executing a nonexistent file: "
./modify.sh --lowercase no.txt

echo "
Test 4 executing uppercase without filename arg: "
./modify.sh -u

echo "
Test 5 executing conversion to uppercase (-u): "
./modify.sh -u filename.txt

echo "
Test 6 executing  conversion to uppercase (--uppercase) without changes: "
./modify.sh --uppercase FILENAME.TXT

echo "
Test 7 executing lowercase without filename arg: "
./modify.sh -l

echo "
Test 8 executing conversion to lowercase (-l): "
./modify.sh -l FILENAME.TXT

echo "
Test 9 executing conversion to lowercase (--lowercase) without changes: "
./modify.sh --lowercase filename.txt

echo "
Test 10 executing conversion to uppercase (-u) with path: "
./modify.sh -u directory1/filename1

echo "
Test 11 executing conversion to lowercase (-l) with path: "
./modify.sh -l directory1/FILENAME1

echo "
Test 12 executing conversion to uppercase (-u) with directory: "
./modify.sh -u directory1/

echo "
Test 13 executing conversion to lowercase (-l) with directory: "
./modify.sh -l directory1/

echo "
Test 14 executing recursive without args: "
./modify.sh -r

echo "
Test 15 executing recursive conversion to uppercase (-r --uppercase): "
./modify.sh -r --uppercase directory1/

echo "
Test 16 executing recursive conversion to lowercase (--recursive -l): "
./modify.sh --recursive -l directory1/

echo "
Test 17 executing recursive conversion to uppercase (-r -u) in different directories: "
./modify.sh -r -u directory1/directory2/directory3/ directory1/

echo "
Test 18 executing recursive conversion to lowercase (--recursive --lowercase) in different directories:"
./modify.sh --recursive --lowercase directory1/directory2/directory3/ directory1/

echo "
Test 19 exectuting a wrong sed pattern: "
./modify.sh s/no directory1/

echo "
Test 20 executing a nonexistent directory: "
./modify.sh s/filename/file/g NoDirectory/

echo "
Test 21 executing sed pattern: "
./modify.sh s/filename/file/g directory1/directory2/directory3/

echo "
Test 22 executing recursive sed pattern: "
./modify.sh -r s/file/filename/g directory1/directory2/

echo "
Test 23 executing recursive sed pattern in different directories: "
./modify.sh -r s/filename/file/g directory1/directory2/directory3/ directory1/
echo "

Test 24 executing recursive sed pattern in different directories: "
./modify.sh -r s/file/filename/g directory1/ directory1/directory2/directory3/ 
