<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);
$change_count=0;

define("treasure", 5);
$valid_treasure=[0, 1, 2, 3];

$dir=new DirectoryIterator("../example/resources/maps");
$files=[];
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$files[]=$filename;
	}
}

sort($files);
say("found ".count($files), $quiet);

$files[]="../example/tile_editor_integration/default-map.json";

foreach($files as $filename) {

	$json=json_decode(file_get_contents($filename));

	say($filename, $quiet);

	$attr=$json->attributes;

	foreach($json->layers as &$layer) {

		if($layer->meta->id!=="things") {

			continue;
		}

		foreach($layer->data as &$thing) {


			if($thing->t != treasure) {

				continue;
			}

			//Not a collectible treasure.
			if(!in_array($thing->a->type, $valid_treasure)) {

				continue;
			}

			$thing->a->type=rand(0, 3);
			++$change_count;
		}
	}

	file_put_contents($filename, json_encode($json));
}
say("all done, changed {$change_count} collectibles", $quiet);
exit(0);
