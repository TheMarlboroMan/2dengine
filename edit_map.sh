#!/bin/bash

if [ "$1" == "" ]
then
	echo "specify a map name, e.g ./edit_map.sh forest_001 [--debug]"
	exit 1
fi

debug="off"
if [ "$2" == "--debug" ]
then
	debug="on"
fi

mapname="/home/daniel/devel/2dengine/example/resources/thedreamingtower/maps/$1.json";
if [ ! -f $mapname ]
then

	echo "map $mapname does not exist, create y/n?"
	read val;

	if [ "$val" == "y" ] || [ "$val" == "Y" ]
	then

		cp /home/daniel/devel/2dengine/example/tile_editor_integration/default-map.json $mapname
	else

		echo "exiting"
		exit 0
	fi
fi

cd dev_scripts
./sync_files.sh
cd ..

#easy map browsing...
cd example/resources/maps 

if [ "on" == "$debug" ]
then 
	/home/daniel/devel/tile_editor/build/build/tile_editor --version | head -1 | grep bin
	if [ 0 -ne $? ]
	then
		echo "tile editor must be built as binary"
		exit 1
	fi
	gdb -ex "catch throw" -ex "run" --args /home/daniel/devel/tile_editor/build/build/tile_editor -w 1200x800 -c /home/daniel/devel/2dengine/example/tile_editor_integration/config.txt -f $mapname
else
	tile_editor -w 1200x800 -c /home/daniel/devel/2dengine/example/tile_editor_integration/config.txt -f $mapname
fi

