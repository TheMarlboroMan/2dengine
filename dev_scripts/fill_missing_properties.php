<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);

define("projectile_generator", 51);
define("timed_trap", 53);
define("moving_block", 55);
define("touch_trigger", 11);
define("exit_item", 2);
define("button", 7);
define("breaking_block", 9);
define("text_node", 12);

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

			switch($thing->t) {

				case projectile_generator:
				case timed_trap:

					if(!property_exists($thing->a, "keep_active_reset")) {

						$thing->a->keep_active_reset=1;
					}
				break;
				case touch_trigger:
				case button:

					if(!property_exists($thing->a, "keep_used_reset")) {

						$thing->a->keep_used_reset=1;
					}

					if(!property_exists($thing->a, "repeatable")
						&& $thing->t===button
					) {

						$thing->a->repeatable=0;
					}
				break;
				case exit_item:
					if(!property_exists($thing->a, "min_rooms")) {

						$thing->a->min_rooms=0;
					}

					if(!property_exists($thing->a, "transition")) {

						$thing->a->transition=0;
					}
				break;
				case moving_block:
					if(!property_exists($thing->a, "solid")) {

						$thing->a->solid=1;
					}

					if(!property_exists($thing->a, "crouch_activated")) {

						$thing->a->crouch_activated=0;
					}
				break;
				case breaking_block:
					if(!property_exists($thing->a, "type")) {

						$thing->a->type=1;
					}
				break;
				case text_node:

					if(!property_exists($thing->a, "answer_1")) {

						$thing->a->answer_1="-";
					}

					if(!property_exists($thing->a, "answer_2")) {

						$thing->a->answer_2="-";
					}

					if(!property_exists($thing->a, "answer_3")) {

						$thing->a->answer_3="-";
					}
				break;
			}
		}
	}

	file_put_contents($filename, json_encode($json));
}
say("all done", $quiet);
exit(0);
