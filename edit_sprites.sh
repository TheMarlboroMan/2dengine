#!/bin/bash
cd dev_scripts
./sync_files.sh
cd ..
sprite-table -i example/resources/bitmaps/bitmap.png -f example/resources/lists/tiles.txt -w 900x600
