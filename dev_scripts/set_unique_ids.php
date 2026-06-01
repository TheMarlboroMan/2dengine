<?php
function say(string $_what, bool $_quiet) : void {

	if($_quiet) {

		return;
	}

	echo $_what.PHP_EOL;
}

$args=getopt("", ["quiet::"]);
$quiet=array_key_exists("quiet", $args);

$collectible_id=0;
$cover_id=0;
$button_id=0;
$trigger_id=0;
$text_id=0;

$collectible_type=5;
$cover_type=6;
$switch_type=7;
$trigger_type=11;
$text_type=12;
$dir=new DirectoryIterator("../example/resources/maps");
$files=[];
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$files[]=$filename;
	}
}

sort($files);

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

				case $collectible_type:

					say("found thing with id {$thing->a->id}", $quiet);
					$thing->a->id=++$collectible_id;
				break;
				case $cover_type:

					say("found cover with id {$thing->a->id}", $quiet);
					$thing->a->id=++$cover_id;
				break;
				case $switch_type:

					say("found button with id {$thing->a->id}", $quiet);
					$thing->a->id=++$button_id;
				break;
				case $trigger_type:

					say("found trigger with id {$thing->a->id}", $quiet);
					$thing->a->id=++$trigger_id;
				break;
				case $text_type:

					say("found text with id {$thing->a->event_id}", $quiet);
					$thing->a->event_id=++$text_id;
				break;
			}
		}
	}

	file_put_contents($filename, json_encode($json));
}

say("total things $collectible_id", $quiet);
say("total covers $cover_id", $quiet);
say("total buttons $button_id", $quiet);
say("total triggers $trigger_id", $quiet);
say("total text nodes $text_id", $quiet);
exit(0);
