<?php
	function get_rover_lat(){
		$jsonUrl="../log/recv.json";
			if (file_exists($jsonUrl)) {
				$MCjson = file_get_contents($jsonUrl);
				//$MCjson = mb_convert_encoding($MCjson, 'UTF8', 'ASCII,JIS,UTF-8,EUC-JP,SJIS-WIN');
				$MCobj = json_decode($MCjson,true);
			} 
			return $MCobj['lat'];
			
	}
	function get_rover_lng(){
		$jsonUrl="../log/recv.json";
			if (file_exists($jsonUrl)) {
				$MCjson = file_get_contents($jsonUrl);
				//$MCjson = mb_convert_encoding($MCjson, 'UTF8', 'ASCII,JIS,UTF-8,EUC-JP,SJIS-WIN');
				$MCobj = json_decode($MCjson,true);
			} 
			return $MCobj['lng'];
			
	}
	function get_rover_mode(){
		$jsonUrl="../log/recv.json";
			if (file_exists($jsonUrl)) {
				$MCjson = file_get_contents($jsonUrl);
				//$MCjson = mb_convert_encoding($MCjson, 'UTF8', 'ASCII,JIS,UTF-8,EUC-JP,SJIS-WIN');
				$MCobj = json_decode($MCjson,true);
			} 
			if($MCobj['mode']=="4"){
				return "FIX";
			}else if($MCobj['mode']=="5"){
				return "FLOAT";
			}
			return "Failure";
			
	}
	function get_rover_dir(){
		$jsonUrl="../log/recv.json";
			if (file_exists($jsonUrl)) {
				$MCjson = file_get_contents($jsonUrl);
				//$MCjson = mb_convert_encoding($MCjson, 'UTF8', 'ASCII,JIS,UTF-8,EUC-JP,SJIS-WIN');
				$MCobj = json_decode($MCjson,true);
			} 
			return $MCobj['dir'];
			
	}
	function get_rover_vision(){
		$cmd = '../sh/get_cam_url.sh';
		$url=exec($cmd);
		return $url;
	}
	$func = $_POST['func'];
	echo $func();
?>
