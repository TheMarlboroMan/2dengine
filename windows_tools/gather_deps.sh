#!/bin/bash

if [ $# -ne 1 ]
then 

	echo "usage ${0} path/where/you/want/all/projects"
	exit 1
fi

cp build_all.sh $1/build_all.sh

cd $1

git clone https://github.com/themarlboroman/log
git clone https://github.com/themarlboroman/tools
git clone https://github.com/themarlboroman/appenv
git clone https://github.com/themarlboroman/console
git clone https://github.com/themarlboroman/libdansdl2
git clone https://github.com/themarlboroman/ldtools
git clone https://github.com/themarlboroman/appconsole
git clone https://github.com/themarlboroman/dfw

echo "now you can go into ${1} and run ./build_all.sh to make all libs"

