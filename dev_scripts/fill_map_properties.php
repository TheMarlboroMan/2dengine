<?php
$collectible_id=0;
$cover_id=0;
$button_id=0;
$trigger_id=0;

$collectible_type=5;
$cover_type=6;
$switch_type=7;
$trigger_type=11;
$dir=new DirectoryIterator("../example/resources/maps");

$files=[
	"../example/tile_editor_integration/default-map.json"
];

foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$files[]="../example/resources/maps/{$fileinfo->getFilename()}";
	}
}

foreach($files as $filename) {

	$json=json_decode(file_get_contents($filename));

	$attr=$json->attributes;

	if(!property_exists($attr, "music_id")) {

		echo "setting music id for {$filename}".PHP_EOL;
		$attr->music_id=0;
	}

	if(!property_exists($attr, "bg_color")) {

		echo "setting bg color for {$filename}".PHP_EOL;
		$attr->bg_color=0;
	}

	if(!property_exists($attr, "in_game")) {

		echo "setting in_game {$filename}".PHP_EOL;
		$attr->in_game=1;
	}

	if(property_exists($attr, "save_point")) {

		echo "removing save point for {$filename}".PHP_EOL;
		unset($attr->save_point);
	}

	if(property_exists($attr, "name")) {

		echo "removing name for {$filename}".PHP_EOL;
		unset($attr->name);
	}

	if(property_exists($attr, "automap_id")) {

		echo "removing automap_id for {$filename}".PHP_EOL;
		unset($attr->automap_id);
	}

	foreach($json->layers as &$layer) {

		if($layer->meta->type!=="things") {

			continue;
		}

		foreach($layer->data as &$thing) {

			if($thing->t!==51) {

				continue;
			}

			if(property_exists($thing, "pre_ms")) {

				unset($thing->pre_ms);
			}

			if(!property_exists($thing->a, "pre_ms")) {

				echo "setting pre_ms for projectile generator".PHP_EOL;
				$thing->a->pre_ms=0;
			}
		}
	}

	file_put_contents($filename, json_encode($json));
}
exit(0);
