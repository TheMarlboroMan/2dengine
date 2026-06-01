<?php
$collectible_id=0;
$cover_id=0;
$button_id=0;

$collectible_type=5;
$cover_type=6;
$switch_type=7;
$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if(!$fileinfo->isDot()) {

		$filename="../example/resources/maps/{$fileinfo->getFilename()}";
		$json=json_decode(file_get_contents($filename));

		$things=null;
		$background=null;
		$middle=null;
		$foreground=null;
		$logic=null;

		echo $filename.PHP_EOL;

		foreach($json->layers as $layer) {
	
			switch($layer->meta->id) {

					case "things": $things=$layer; break;
					case "background": $background=$layer; break;
					case "middle": $middle=$layer; break;
					case "foreground": $foreground=$layer; break;
					case "logic": $logic=$layer; break;
			}
		}

		$json->layers=[$things, $foreground, $middle, $background, $logic];
		file_put_contents($filename, json_encode($json));
    }
}

echo "maps redone".PHP_EOL;
exit(0);
