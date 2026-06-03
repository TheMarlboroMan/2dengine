#!/bin/bash

php set_unique_ids.php --quiet
mkdir ../build/resources
cp -r -v ../example/resources/* ../build/resources

