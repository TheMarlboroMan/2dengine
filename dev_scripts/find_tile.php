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

$args=getopt("", ["id:", "quiet::"]);

if(!array_key_exists("id", $args)) {

	echo "find_tile.php --id tileid [--quiet]".PHP_EOL;
	exit(1);
}

$quiet=array_key_exists("quiet", $args);
$id=(int)$args["id"];

$decoration_layers=["foreground", "background", "middle"];

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
				if($type===$id) {

					echo $filename.PHP_EOL;
					break 2;
				}
			}
		}
    }
}

exit(0);
