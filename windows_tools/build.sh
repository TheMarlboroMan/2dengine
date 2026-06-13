#!/bin/bash

echo "This will only work if the drive is C: and msys2 is in C:/msys64..."
sleep 3

function build_dependency {

	cd $1

	branch=$(git branch --show-current)
	git reset --hard HEAD
	git pull origin $branch
	if [ -d build ]
	then 
		rm -rf build
	fi

	mkdir build
	cd build
	cmake .. -G "MSYS Makefiles" -DBUILD_STATIC=ON -DBUILD_SHARED=ON -DBUILD_DEBUG=ON
	make -j2

	cd ../..
}

destination="../.."

echo "cloning dependencies..."

cd $destination
git clone https://github.com/themarlboroman/log
git clone https://github.com/themarlboroman/tools
git clone https://github.com/themarlboroman/appenv
git clone https://github.com/themarlboroman/console
git clone https://github.com/themarlboroman/libdansdl2
git clone https://github.com/themarlboroman/ldtools
git clone https://github.com/themarlboroman/appconsole
git clone https://github.com/themarlboroman/dfw

echo "building dependencies..."
build_dependency log
build_dependency tools
build_dependency appenv
build_dependency console
build_dependency libdansdl2
build_dependency ldtools
build_dependency appconsole
build_dependency dfw

echo "building the project"
mkdir ../build
cd ../build
cmake .. -G "MSYS Makefiles"
make -j2

echo "bundling resources"

cp -r ../example/resources resources
unix2dos resources/localization/en/*

echo "gathering built libs"
cp ../../log/build/*.dll ./
cp ../../tools/build/*.dll ./
cp ../../appenv/build/*.dll ./
cp ../../console/build/*.dll ./
cp ../../libdansd2l/build/*.dll ./
cp ../../ldtools/build/*.dll ./
cp ../../appconsole/build/*.dll ./
cp ../../dfw/build/*.dll ./

echo "gathering system dlls"

for file in `ntldd -R the-dreaming-tower.exe | grep -v "ext-ms" | grep -v "PdmUtil" | grep -v "HvsiFile" | grep "not found | awk '{print $1}'`
do
	cp /c/msys64/mingw64/bin/$file ./
done

