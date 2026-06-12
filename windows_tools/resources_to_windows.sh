#!/bin/bash

#this will convert all resource files to windows into the given build directory

if [ $# -ne 1 ]
then 

	echo "usage ${0} relative/build/path"
	exit 1
fi

cp -r ../resources ${1}/resources
cd ${1}/resources
unix2dos localization/en/*
