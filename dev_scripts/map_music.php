<?php
if(3!==$argc) {

	echo "use {$argv[0]} mapname musicid where mapname is like start_ or red_".PHP_EOL;
	exit(1);
}

$mapname=$argv[1];
$musicid=(int)$argv[2];

$mapname_len=strlen($mapname);
$files=[];

$dir=new DirectoryIterator("../example/resources/maps");
foreach($dir as $fileinfo) {

	if($fileinfo->isDot()) {

		echo "skipping {$fileinfo->getFilename()}".PHP_EOL;
		continue;
	}

	if(substr($fileinfo->getFilename(), 0, $mapname_len)==$mapname) {

		$files[]="../example/resources/maps/{$fileinfo->getFilename()}";
	}
}

foreach($files as $filename) {

	$json=json_decode(file_get_contents($filename));

	$attr=$json->attributes;
	echo "setting music id for {$filename}".PHP_EOL;
	$attr->music_id=$musicid;
	file_put_contents($filename, json_encode($json));
}
exit(0);
