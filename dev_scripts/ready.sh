#!/bin/bash

with_ids="";

if [ "--with_ids" == "${1}" ]
then 
	with_ids="on";
elif [ "--no_ids" == "${1}" ]
then
	with_ids="off"
else
	echo "usage ${0} --with_ids|--no_ids"
	exit 1
fi

if [ "on" == "${with_ids}" ]
then
	php set_unique_ids.php
fi

./make_resources.sh
