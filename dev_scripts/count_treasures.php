<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);

$total_count_easy=0;
$total_count_medium=0;
$total_count_hard=0;

define("easy", 1);
define("medium", 2);
define("hard", 4);

$collectible_type=5;
$valid_treasure=[0, 1, 2, 3];
$dir=new DirectoryIterator("../example/resources/thedreamingtower/maps");
foreach($dir as $fileinfo) {

	if($fileinfo->isDot()) {

		continue;
	}

	$filename="../example/resources/thedreamingtower/maps/{$fileinfo->getFilename()}";

	if(0===strpos($fileinfo->getFilename(), "test")) {

		echo "skipping {$filename}".PHP_EOL;
		continue;
	}

	$json=json_decode(file_get_contents($filename));

	$attr=$json->attributes;

	foreach($json->layers as &$layer) {

		if($layer->meta->id!=="things") {

			continue;
		}

		foreach($layer->data as &$thing) {

			switch($thing->t) {

				case $collectible_type:

					if(in_array($thing->a->type, $valid_treasure)) {

						$skill=$thing->a->difficulty;

						if($skill & easy) {

							++$total_count_easy;
						}

						if($skill & medium) {

							++$total_count_medium;
						}

						if($skill & hard) {

							++$total_count_hard;
						}
					}
				break;
			}
		}
	}
}
echo <<<R
easy:{$total_count_easy}
medium:{$total_count_medium}
hard:{$total_count_hard}

R;

exit(0);

