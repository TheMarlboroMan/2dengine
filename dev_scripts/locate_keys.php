<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);

$collectible_type=5;
$dir=new DirectoryIterator("../example/resources/thedreamingtower/maps");
foreach($dir as $fileinfo) {

	if($fileinfo->isDot()) {

		continue;
	}

	if(0===strpos($fileinfo->getFilename(), "test")) {

		echo "skipping {$filename}".PHP_EOL;
		continue;
	}

	$filename="../example/resources/thedreamingtower/maps/{$fileinfo->getFilename()}";
	$json=json_decode(file_get_contents($filename));

	$attr=$json->attributes;

	foreach($json->layers as &$layer) {

		if($layer->meta->id!=="things") {

			continue;
		}

		foreach($layer->data as &$thing) {

			switch($thing->t) {

				case $collectible_type:

					if($thing->a->type==10) {

						say("found thing with id {$thing->a->id} in $filename", $quiet);
					}
				break;
			}
		}
	}
}
exit(0);
