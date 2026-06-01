<?php
$collectible_id=0;
$cover_id=0;
$button_id=0;

$collectible_type=5;
$cover_type=6;
$switch_type=7;
$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	$new_layer=(object)[
		"meta" => (object) [
			"set" => 2,
			"gridset" => 1,
			"alpha" => 255,
			"type" => "tiles",
			"id" => "middle"
		],
		"data" => []
	];

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$json=json_decode(file_get_contents($filename));
		$json->layers[]=$new_layer;
		file_put_contents($filename, json_encode($json));
    }
}

echo "maps redone".PHP_EOL;
exit(0);
