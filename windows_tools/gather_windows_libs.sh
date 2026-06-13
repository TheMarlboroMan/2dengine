#!/bin/bash

if [ $# -ne 1 ]
then 

	echo "usage ${0} relative/build/path"
	exit 1
fi

cd $1

º
echo "copying all libs from local builds..."
cp ../../*/build/*.dll ./

echo "you still need to do ntldd -R executable.file | grep -v "ms-ext" | grep "not found" and fix those";
