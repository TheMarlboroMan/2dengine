#!/bin/bash

echo "copying all libs from local builds..."
cp ../../*/build/*.dll ./

echo "running ldd to find the rest..."
ldd the-dreaming-tower.exe | awk {print $1} > libs.txt
for file in $(cat libs.txt)
do

	location="/c/msys64/mingw64/bin/$file"

	if [ -f $location ]
	then
		cp $location ./
	fi
done
