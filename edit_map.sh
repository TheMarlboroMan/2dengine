#!/bin/bash

if [ "$1" == "" ]
then
	echo "specify a map name, e.g ./edit_map.sh forest_001"
	exit 1
fi

mapname="/home/daniel/devel/2dengine/example/resources/maps/$1.json";
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
tile_editor -w 1200x800 -c /home/daniel/devel/2dengine/example/tile_editor_integration/config.txt -f $mapname
