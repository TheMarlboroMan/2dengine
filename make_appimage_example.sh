#!/bin/bash

if [[ $# -ne 2 ]]
then
	echo "usage: make_appimage.sh path_to_executable path_to_linuxdeploy"
	exit 1
fi;

#export SIGN=1
#export SIGN_KEY=0xLAST8CHARSOFGPGKEYID
#Well, not really, but hey...
MAJOR_VERSION=0
MINOR_VERSION=0
PATCH_VERSION=0

export VERSION="$MAJOR_VERSION.$MINOR_VERSION.$PATCH_VERSION"
echo "will build $VERSION"
$2 --appdir AppDir
cp -r example/resources/* AppDir/usr/share
mkdir -p AppDir/usr/share/metainfo
cp example/linuxdeploy/*.xml AppDir/usr/share/metainfo/
$2 --executable $1 --appdir AppDir -i ./example/linuxdeploy/icon.png -d ./example/linuxdeploy/app.desktop --output appimage
rm -rf AppDir
exit 0
