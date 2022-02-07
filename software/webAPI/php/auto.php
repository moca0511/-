<!DOCTYPE HTML>
<html>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover">
	<meta http-equiv="Content-Security-Policy" content="default-src * data: gap: https://ssl.gstatic.com; style-src * 'unsafe-inline'; script-src * 'unsafe-inline' 'unsafe-eval'">
	<script src="../components/loader.js"></script>
	<script src="../lib/onsenui/js/onsenui.min.js"></script>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
	
	
	<script>
		var rover_lat=-1.0;
		var rover_lng=-1.0;
		var RoverLatlng;
		var target_lat=-1.0;
		var target_lng=-1.0;
		var targetLatlng;
		var rover_mode=0;
		var rover_dir=0;
		var rover_vision=0;
		
		const KCG_RAKUHOKU_LAT = 35.04423053007073;
		const KCG_RAKUHOKU_LNG = 135.7722187666102;


		var my_lat=-1.0;
		var my_lng=-1.0;
		
		var rover_marker,map,target_marker;

		function getMyLatlng(){
				
					my_lat=KCG_RAKUHOKU_LAT;
					my_lng=KCG_RAKUHOKU_LNG;
				
			}

		async function waitMs(wait_time){
				const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
				await _sleep(wait_time);
		}

		async function initMap() {
			stop_rover();
			getRoverLatlng();
			jQuery.ajax({
				
				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {'func' : 'get_rover_vision' }, //POSTするデータ
				success: function(content){ //正常に処理が完了した時
					
					rover_vision=content;
					console.log(rover_vision);
					
				}
			 
			});
			//document.getElementById('console').innerHTML = "<img src= " + rover_vison +"; altonerror=this.onerror = null; this.src='';>";
			//位置情報取得待ち
			while(rover_lat==-1.0||rover_lng==-1.0||rover_vision==0){
					const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
					await _sleep(100);
			}

			console.log( "中央座標\n[" + rover_lat + "," + rover_lng + "]\nでmap表示" ) ;
			document.getElementById('vision').innerHTML = "<img src= " + rover_vision +"; altonerror=this.onerror = null; this.src='';width='100%' height=auto>";
			map = new google.maps.Map(document.getElementById("map"), {
				center: { lat: rover_lat, lng: rover_lng},
				zoom: 17
			});
			//マーカーの初期プロパティを指定
			 rover_marker = new google.maps.Marker({
			  position: RoverLatlng,
			  map: map,
			  icon: {
				url: '../pic/panda_icon_144_144.png',                      //アイコンのURL
				scaledSize: new google.maps.Size(40, 40) //サイズ
			},
			//optimized: false             //★写真を丸くするために「false」を指定する
			 });
			 target_marker = new google.maps.Marker({
			  position: targetLatlng,
			  map: map
			 });
			 
			 
			rover_marker.setMap(map);
			target_marker.setMap(null);
			
			// クリックイベントを追加
			map.addListener('click', function(e) {
				setMarker(e.latLng, map);
			});
			while(1){
				const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
					await _sleep(1000);
				getRoverLatlng();
				console.log(RoverLatlng + "," + rover_marker.position)
				if(!(RoverLatlng.equals(rover_marker.position))){
					rover_marker.setMap(null);
					rover_marker.position=RoverLatlng;
					rover_marker.setMap(map);
				}
			}	
			
		}

		function　mapCenterToRover(){
			map.panTo(rover_marker.position);
			map.setZoom(20);
		}
		function　mapCenterToTarget(){
			map.panTo(target_marker.position);
			map.setZoom(20);
		}
		function setMarker(lat_lng, map) {
			// //マーカー削除
			//   MarkerClear();

			console.log("lat:"+lat_lng.lat()+"\nlng:"+lat_lng.lng()+"\n");
			target_marker.setMap(null);
			targetLatlng=lat_lng;
			target_marker.position=targetLatlng;
			target_marker.setMap(map);
			
			document.getElementById('lat-id').value = targetLatlng.lat();
			document.getElementById('lng-id').value = targetLatlng.lng();
			// 座標の中心をaずらす
			// http://syncer.jp/google-maps-javascript-api-matome/map/method/panTo/
			map.panTo(lat_lng);
		}
		
		function set_target_pin(){
			
			target_marker.setMap(null);
			targetLatlng=new google.maps.LatLng(parseFloat(document.getElementById('lat-id').value), parseFloat(document.getElementById('lng-id').value));
			console.log("target=" + targetLatlng);
			target_marker.position=targetLatlng;
			target_marker.setMap(map);
			mapCenterToTarget();
		}
	
		function getRoverLatlng(){
			console.log("test");
			jQuery.ajax({
				
				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {'func' : 'get_rover_lat' }, //POSTするデータ
				success: function(content){ //正常に処理が完了した時
					
					rover_lat=parseFloat(content);
					console.log(rover_lat);
					
				}
			 
			});
			jQuery.ajax({
				
				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {'func' : 'get_rover_lng' }, //POSTするデータ
				success: function(content){ //正常に処理が完了した時
					
					rover_lng=parseFloat(content);
					console.log(rover_lng);
					
				}
			 
			});jQuery.ajax({
				
				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {'func' : 'get_rover_mode' }, //POSTするデータ
				success: function(content){ //正常に処理が完了した時
					
					rover_mode=content;
					console.log(rover_mode);
					
				}
			 
			});
			jQuery.ajax({
				
				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {'func' : 'get_rover_dir' }, //POSTするデータ
				success: function(content){ //正常に処理が完了した時
					
					rover_dir=parseFloat(content);
					console.log(rover_dir);
					
				}
			 
			});
			RoverLatlng = new google.maps.LatLng(rover_lat, rover_lng);
			document.getElementById('console').innerHTML = "<p>RTK="+rover_mode+",dir="+rover_dir+"<br>lat="+rover_lat+",lng="+rover_lng+"</p>";
			//var rover_Latlng = new google.maps.LatLng(lat,lng);
			//setMarker(rover_Latlng, map);
		}
	</script>
	

	<link rel="stylesheet" href="../components/loader.css">
	<link rel="stylesheet" href="../lib/onsenui/css/onsenui.css">
	<link rel="stylesheet" href="../lib/onsenui/css/onsen-css-components.css">
	<link rel="stylesheet" href="../css/style.css">
	<link rel="manifest" href="../manifest.json">

	<script async src="https://cdn.jsdelivr.net/npm/pwacompat@2.0.7/pwacompat.min.js"></script>

	<script>
		ons.ready(function() {
		console.log("Onsen UI is ready!");
		});

		if (ons.platform.isIPhoneX()) {
		document.documentElement.setAttribute('onsflag-iphonex-portrait', '');
		document.documentElement.setAttribute('onsflag-iphonex-landscape', '');
		}
	</script>

	<style>

		#vision_console {
			height: 100%;
			width: 50%;
			float : left;
			background:black;
			color:white;
			/* display: flex;
			text-align: center;
			align-items: center;
			justify-content: center; */
		}
		#vision{
			width: 100%;
			height: 75%;
			
		}
		#vision img{
			width: 100%;
			height: auto;
			/* object-fit: cover; */
		}
		#console{
			width: 100%;
			height: 20%;
			font-size:110%;
		}

		#map {
			height: 75%;
			width: 50%;
			float : right;
			font-size:200%;
			text-align: center;
			display: flex;
			align-items: center;
			justify-content: center
		}
		
		#map p{
		margin: 0;
		}
		#target_area {
			height: 25%;
			width: 50%;
			float : right;
			background:white;
			color:black;
		}
		#target{
			padding: 0px 10px 10px 10px;
		}
		input[type="text"],
		textarea {
			width: 60%;
		}
		#latlng{
			width: 70%;
			hight: 100%;
			float : left;
		}
		#button_area button{
			margin: 5px;
		}
		
	</style>

	<script>
		if ('serviceWorker' in navigator) {
		navigator.serviceWorker.register('../service-worker.js').then(function (registration) {
			console.log('ServiceWorker registration successful with scope: ', registration.scope);
		}).catch(function (err) {
			console.log('ServiceWorker registration failed: ', err);
		});
		}
	</script>
	<script>
		function stop_rover(){
			console.log('stop_rover');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			// sOrAField.value = "start";
			stateField.value = "0";
			document.frmMain.submit();
  			return false;
		}
		function move_forward_rover(){
			console.log('move_forward_rover');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			// sOrAField.value = "start";
			// stateField.value = "stop_rover";
			stateField.value = "1";
			document.frmMain.submit();
  			return false;
		}
		function move_back_rover(){
			console.log('move_back_rover');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			stateField.value = "2";
			// sOrAField.value = "start";
			// stateField.value = "stop_rover";
			document.frmMain.submit();
  			return false;
		}
		function turn_right_rover(){
			console.log('turn_right_rover');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			// sOrAField.value = "start";
			stateField.value = "3";
			document.frmMain.submit();
  			return false;
		}
		function turn_left_rover(){
			console.log('turn_left_rover');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			// sOrAField.value = "start";
			stateField.value = "4";
			document.frmMain.submit();
  			return false;
		}
		function send_target(){
			console.log('send_target');
			const ymdField = document.getElementById('ymd-id');
			const timeField = document.getElementById('time-id');
			// const sOrAField = document.getElementById('start_arrival-id');
			const stateField = document.getElementById('state-id');
			ymdField.value = "0000/00/00";
			timeField.value = "00:00:00";
			// sOrAField.value = "start";
			stateField.value = "5";
			document.frmMain.submit();
		}
	</script>

	</head>
	<body>
		
		<ons-page>
			<ons-toolbar>
				<div class="center" onmouseup="location.href='./main.php'">
					<img src= '../pic/panda_yoko.png' height="100%">
				</div>
				<div class="right">
					<ons-toolbar-button>
						<ons-icon icon="fa-gamepad" onclick="location.href='./manual.php'"></ons-icon>
					</ons-toolbar-button>
					<ons-toolbar-button>
						<ons-icon icon="fa-map-marker-alt" onclick="location.href='./auto.php'"></ons-icon>
					</ons-toolbar-button>
						<ons-toolbar-button onclick="stop_rover();">
							<ons-icon style="color: red;" icon="fa-ban"></ons-icon>
						</ons-toolbar-button>
					
				</div>
			</ons-toolbar>
			
			<div id="vision_console">
				<div id="vision">
					
				</div>
				<div id="console">
					
				</div>
			</div>
			<div id="map">
				<div>
					<ons-icon icon="spinner" spin="true" size="2x"></ons-icon>
					<p>Location information acquisition</p>
				</div>
			</div>
			
		

	
			<div id="target_area">
				
				<div id="target">
					<div id="latlng">
						<form action="./log.php" method="POST" name="frmMain" target="sendPhoto">
							<pre  style="font-size: 105%;">lat <input type="text" name="lat" id="lat-id" value="35.04423053007073"></pre>
							<pre style="font-size: 105%;">lng <input type="text" name="lng" id="lng-id" value="135.7722187666102"></pre>
							<input type="hidden" name="state" id="state-id" value="5">
							<input type="hidden" name="ymd" id="ymd-id" value="0000/00/00">
							<input type="hidden" name="time" id="time-id" value="00:00:00">
							<!-- <input type="hidden" name="start_arrival" id="start_arrival-id" value="start"> -->
						</form>
					</div>
					<div id="button_area">
						<button id="pan_target"onclick="mapCenterToTarget();">target</button>
						<button id="pan_rover"onclick="mapCenterToRover();">rover</button>
						<br>
						<button id="set_target"onclick="set_target_pin();">set</button>
						<button id="set_target"onclick="send_target();">go</button>
					</div>
				</div>
			</div>
		</ons-page>
		
	<iframe name="sendPhoto" style="width:0px;height:0px;border:0px;"></iframe>
	<script src="https://maps.googleapis.com/maps/api/js?key=APIkey&callback=initMap" async defer></script>
	</body>
</html>