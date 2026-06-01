<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

function is_exception(int $_type) : bool {

	return false;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);


$decoration_layers=["foreground", "background", "middle"];

$tiles=[]; //a map of tile id to count of times it appears...
$tilefile=file("../example/resources/lists/tiles.txt");
foreach($tilefile as $data) {

	$pieces=explode("\t", $data);
	$id=(int)array_shift($pieces);

	//Items...
	if($id >= 300 && $id <=474) {

		continue;
	}

	//Wall animations!
	if(in_array($id, [216, 217, 501, 502, 505, 506, 507, 509, 510])) {

		continue;
	}

	//Misplaced particles!
	if($id >= 514 && $id <= 600) {

		continue;
	}

	$tiles[$id]=0;
}

$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$json=json_decode(file_get_contents($filename));

		$attr=$json->attributes;

		foreach($json->layers as &$layer) {

			if(!in_array($layer->meta->id, $decoration_layers)) {

				continue;
			}

			foreach($layer->data as $tile) {

				$type=$tile->t;
				if(!array_key_exists($type, $tiles)) {

					say("Tile {$type} is not registered in {$filename}!!", $quiet);
					$tiles[$type]=0;
				}

				++$tiles[$type];
			}
		}
    }
}

//Good, now sort xD!
asort($tiles);
print_r($tiles);

exit(0);
