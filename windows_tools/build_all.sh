#!/bin/bash

function build_project {

	cd $1

	branch=$(git branch --show-current)
	git reset --hard HEAD
	git pull origin $branch
	rm -rf build
	mkdir build
	cd build
	cmake .. -G "MSYS Makefile" -DBUILD_STATIC=ON -DBUILD_SHARED=ON -DBUILD_DEBUG=ON
	make
}

build_project log
build_project tools
build_project appenv
build_project console
build_project libdansdl2
build_project ldtools
build_project appconsole
build_project dfw
