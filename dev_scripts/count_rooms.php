<?php
$total=0;
$discoverable=0; //appears on map, counted
$non_discoverable=0; //dream sequences, not counted but played
$non_playable=0; //intros and endings
$per_area=[];

$nowhere_areas=["red_key", "blue_key", "green_key"];

$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if($fileinfo->isDot()) {

		continue;
	}

	$filename="../example/resources/maps/{$fileinfo->getFilename()}";

	if(0===strpos($fileinfo->getFilename(), "test")) {

		echo "skipping {$filename}".PHP_EOL;
		continue;
	}

	$last_unsderscore_pos=strrpos($fileinfo->getFilename(), "_");
	if(false===$last_unsderscore_pos) {

		echo "skipping {$filename} as bad name".PHP_EOL;
		continue;
	}

	$area=substr($fileinfo->getFilename(), 0, $last_unsderscore_pos);

	$json=json_decode(file_get_contents($filename));
	$attr=$json->attributes;
	$in_game=$attr->in_game;
	++$total;

	if(!array_key_exists($area, $per_area)) {

		$per_area[$area]=[
			"discoverable" => 0,
			"non_discoverable" => 0,
			"non_playable" => 0
		];
	}

	$in_nowhere=in_array($area, $nowhere_areas);

	if($in_game) {

		$in_nowhere
			? ++$non_discoverable
			: ++$discoverable;

		$in_nowhere
			? ++$per_area[$area]["non_discoverable"]
			: ++$per_area[$area]["discoverable"];
	}
	else {

		++$non_playable;
		++$per_area[$area]["non_playable"];
	}
}

$total_playable=$discoverable+$non_discoverable;

echo <<<R
total: {$total}
	discoverable: {$discoverable}
	non_discoverable: {$non_discoverable}
	playable: {$total_playable}
	non_playable: {$non_playable}

per area:

R;

foreach($per_area as $area => $data) {

	$discoverable=$data["discoverable"];
	$non_discoverable=$data["non_discoverable"];
	$total_playable=$discoverable+$non_discoverable;
	$non_playable=$data["non_playable"];

	echo <<<R
	{$area}:
		discoverable: {$discoverable}
		non_discoverable: {$non_discoverable}
		playable: {$total_playable}
		non_playable: {$non_playable}

R;
}

exit(0);
