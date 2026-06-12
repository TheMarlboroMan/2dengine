#!/bin/bash

if [ $# -ne 1 ]
then 

	echo "usage ${0} path/where/you/want/all/projects"
	exit 1
fi

cat > "${1}/build_all.sh" << EOM
#!/bin/bash

function build_project {

	cd \$1

	branch=\$(git branch --show-current)
	git reset --hard HEAD
	git pull origin \$branch
	if [ -d build ]
	then 
		rm -rf build
	fi

	mkdir build
	cd build
	cmake .. -G "MSYS Makefiles" -DBUILD_STATIC=ON -DBUILD_SHARED=ON -DBUILD_DEBUG=ON
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
EOM

cd $1

git clone https://github.com/themarlboroman/log
git clone https://github.com/themarlboroman/tools
git clone https://github.com/themarlboroman/appenv
git clone https://github.com/themarlboroman/console
git clone https://github.com/themarlboroman/libdansdl2
git clone https://github.com/themarlboroman/ldtools
git clone https://github.com/themarlboroman/appconsole
git clone https://github.com/themarlboroman/dfw

chmod +x ${1}/build_all.sh
echo "now you can go into ${1} and run ./build_all.sh to make all libs"

