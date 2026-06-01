#!/bin/bash

cd dev_scripts
./sync_files.sh
cd ..
animation_editor -i example/resources/bitmaps/bitmap.png -s example/resources/lists/tiles.txt -f example/resources/lists/animations.txt
