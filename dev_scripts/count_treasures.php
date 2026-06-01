<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);

$total_count=0;
$collectible_type=5;
$valid_treasure=[0, 1, 2, 3];
$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
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

							++$total_count;
						}
					break;
				}
			}
		}
    }
}
echo $total_count.PHP_EOL;
exit(0);

