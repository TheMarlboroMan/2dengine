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

$args=getopt("", ["from:", "to:", "quiet::"]);

$in_ok=array_key_exists("from", $args) && array_key_exists("to", $args);
if(!$in_ok) {

	echo "replace_tile.php --from tileid --to tileid [--quiet]".PHP_EOL;
	exit(1);
}

$quiet=array_key_exists("quiet", $args);
$from=(int)$args["from"];
$to=(int)$args["to"];

$decoration_layers=["foreground", "background", "middle"];

$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$must_rewrite=false;

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$json=json_decode(file_get_contents($filename));

		$attr=$json->attributes;

		foreach($json->layers as &$layer) {

			if(!in_array($layer->meta->id, $decoration_layers)) {

				continue;
			}

			foreach($layer->data as $tile) {

				if($tile->t===$from) {

					$tile->t=$to;
					$must_rewrite=true;
				}
			}
		}

		if($must_rewrite) {

			file_put_contents($filename, json_encode($json));
		}
    }
}

exit(0);
