<?php
$contents=file("../example/resources/lists/tiles.txt");

$tile_id=1;
$in_gap=false;

while(count($contents)) {

	$line=array_shift($contents);
	$pieces=explode("\t", $line);
	$id=array_shift($pieces);

	if($id!=$tile_id) {

		if(!$in_gap) {

			echo "gap found at $id expected $tile_id".PHP_EOL;
			$in_gap=true;
		}
	}

	if($in_gap && $id==$tile_id) {

		$in_gap=false;
	}

	++$tile_id;
}

