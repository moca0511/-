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
		var rover_lat = -1.0; //rover緯度経度
		var rover_lng = -1.0;
		var RoverLatlng;
		var target_lat = -1.0; //目標座標緯度経度
		var target_lng = -1.0;
		var targetLatlng;
		var rover_mode = 0; //走行モード
		var rover_dir = 0; //ローバ進行方向
		var rover_vision = 0; //カメラストリーミングURL

		const KCG_RAKUHOKU_LAT = 35.04423053007073;
		const KCG_RAKUHOKU_LNG = 135.7722187666102;

		var rover_marker, map, target_marker;

		//マップ初期化関数
		async function initMap() {
			stop_rover();
			//ローバ各種パラメータ取得
			getRoverLatlng();
			jQuery.ajax({

				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {
					'func': 'get_rover_vision'
				}, //POSTするデータ
				success: function(content) { //正常に処理が完了した時

					rover_vision = content;
					console.log(rover_vision);

				}

			});
			//document.getElementById('console').innerHTML = "<img src= " + rover_vison +"; altonerror=this.onerror = null; this.src='';>";
			//位置情報取得待ち
			while (rover_lat == -1.0 || rover_lng == -1.0 || rover_vision == 0) {
				const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
				await _sleep(100);
			}
			//ローバ座標を中央座標としてマップ表示
			console.log("中央座標\n[" + rover_lat + "," + rover_lng + "]\nでmap表示");
			document.getElementById('vision').innerHTML = "<img src= " + rover_vision + "; altonerror=this.onerror = null; this.src='';width='100%' height=auto>";
			map = new google.maps.Map(document.getElementById("map"), {
				center: {
					lat: rover_lat,
					lng: rover_lng
				},
				zoom: 17
			});
			//ローバマーカーの初期プロパティを指定
			rover_marker = new google.maps.Marker({
				position: RoverLatlng,
				map: map,
				icon: {
					url: '../pic/panda_icon_144_144.png', //アイコンのURL
					scaledSize: new google.maps.Size(40, 40) //サイズ
				},
				//optimized: false             //★写真を丸くするために「false」を指定する
			});
			//目標座標マーカーの初期プロパティを指定
			target_marker = new google.maps.Marker({
				position: targetLatlng,
				map: map
			});

			rover_marker.setMap(map);
			// クリックイベントを追加
			map.addListener('click', function(e) {
				mapCenterToRover(); //ローバ座標を中央座標としてマップ再表示
			});
			//1秒周期でローバ各種パラメータ取得
			while (1) {
				const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
				await _sleep(1000);
				getRoverLatlng();
				console.log(RoverLatlng + "," + rover_marker.position)
				if (!(RoverLatlng.equals(rover_marker.position))) {
					rover_marker.setMap(null);
					rover_marker.position = RoverLatlng;
					rover_marker.setMap(map);
				}
			}

		}
		//ローバ座標をマップの中央座標として再表示
		function mapCenterToRover() {
			map.panTo(rover_marker.position);
			map.setZoom(20);
		}
		//目標座標をマップの中央座標として再表示
		function mapCenterToTarget() {
			map.panTo(target_marker.position);
			map.setZoom(20);
		}

		//ローバの各種パラメータ取得
		function getRoverLatlng() {
			console.log("test");
			jQuery.ajax({

				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {
					'func': 'get_rover_lat'
				}, //POSTするデータ
				success: function(content) { //正常に処理が完了した時

					rover_lat = parseFloat(content);
					console.log(rover_lat);

				}

			});
			jQuery.ajax({

				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {
					'func': 'get_rover_lng'
				}, //POSTするデータ
				success: function(content) { //正常に処理が完了した時

					rover_lng = parseFloat(content);
					console.log(rover_lng);

				}

			});
			jQuery.ajax({

				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {
					'func': 'get_rover_mode'
				}, //POSTするデータ
				success: function(content) { //正常に処理が完了した時

					rover_mode = content;
					console.log(rover_mode);

				}

			});
			jQuery.ajax({

				type: 'post',
				url: '../php/get_rover_state.php', //送信先PHPファイル
				data: {
					'func': 'get_rover_dir'
				}, //POSTするデータ
				success: function(content) { //正常に処理が完了した時

					rover_dir = parseFloat(content);
					console.log(rover_dir);

				}

			});
			RoverLatlng = new google.maps.LatLng(rover_lat, rover_lng);
			document.getElementById('console').innerHTML = "<p>RTK=" + rover_mode + ",dir=" + rover_dir + "<br>lat=" + rover_lat + ",lng=" + rover_lng + "</p>";
			//var rover_Latlng = new google.maps.LatLng(lat,lng);
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
			float: left;
			background: black;
			color: white;
			/* display: flex;
			text-align: center;
			align-items: center;
			justify-content: center; */
		}

		#vision {
			width: 100%;
			height: 75%;
			/* object-fit: cover; */
		}

		#vision img {
			width: 100%;
			height: auto;
			/* object-fit: cover; */
		}

		#console {
			width: 100%;
			height: 20%;
			font-size: 110%;
		}

		#map {
			height: 100%;
			width: 50%;
			float: right;
			font-size: 200%;
			text-align: center;
			display: flex;
			align-items: center;
			justify-content: center
		}

		#map p {
			margin: 0;
		}
	</style>

	<script>
		if ('serviceWorker' in navigator) {
			navigator.serviceWorker.register('../service-worker.js').then(function(registration) {
				console.log('ServiceWorker registration successful with scope: ', registration.scope);
			}).catch(function(err) {
				console.log('ServiceWorker registration failed: ', err);
			});
		}
	</script>
	<script>
		//ローバのへの操作命令POST
		function stop_rover() {
			console.log('POST');
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
	</script>

</head>

<body>
	<form action="./log.php" method="POST" name="frmMain" target="sendPhoto">
		<input type="hidden" name="state" id="state-id" value="stop_rover">
		<input type="hidden" name="ymd" id="ymd-id" value="0000/00/00">
		<input type="hidden" name="time" id="time-id" value="00:00:00">
		<input type="hidden" name="lat" id="lat-id" value="0">
		<input type="hidden" name="lng" id="lng-id" value="0">
		<!-- <input type="hidden" name="start_arrival" id="start_arrival-id" value="start"> -->
	</form>
	<ons-page>
		<ons-toolbar>
			<div class="center" onmouseup="location.href='./main.php'">
				<img src='../pic/panda_yoko.png' height="100%">
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

	</ons-page>

	<iframe name="sendPhoto" style="width:0px;height:0px;border:0px;"></iframe>

	<script src="https://maps.googleapis.com/maps/api/js?key=APIKey&callback=initMap" async defer></script>
</body>

</html>