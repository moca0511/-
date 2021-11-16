<!DOCTYPE HTML>
<html>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover">
	<meta http-equiv="Content-Security-Policy" content="default-src * data: gap: https://ssl.gstatic.com; style-src * 'unsafe-inline'; script-src * 'unsafe-inline' 'unsafe-eval'">
	<script src="../components/loader.js"></script>
	<script src="../lib/onsenui/js/onsenui.min.js"></script>
	<script src="../js/map.js"></script>
	

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
		.main {
			background:white;
			color:black;
		}
		#pad button{

		}
		#up{

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
			stateField.value = "stop";
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
			stateField.value = "move_forward";
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
			stateField.value = "move_back";
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
			stateField.value = "turn_right";
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
			stateField.value = "turn_left";
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
			<!-- <input type="hidden" name="start_arrival" id="start_arrival-id" value="start"> -->
		</form>
		<button id="up" ontouchstart="move_forward_rover();" onmousedown="move_forward_rover();" onclick="stop_rover();" ontouchcancel="stop_rover();" ontouchend="stop_rover();">↑</button>
		<button id="down" ontouchstart="move_back_rover();"onmousedown="move_back_rover();" onclick="stop_rover();" ontouchcancel="stop_rover();" ontouchend="stop_rover();">↓</button>
		<button id="right" ontouchstart="turn_right_rover();"onmousedown="turn_right_rover();" onclick="stop_rover();" ontouchcancel="stop_rover();" ontouchend="stop_rover();">→</button>
		<button id="left" ontouchstart="turn_left_rover();"onmousedown="turn_left_rover();" onclick="stop_rover();" ontouchcancel="stop_rover();" ontouchend="stop_rover();">←</button>
		<button onclick="stop_rover();">
			<ons-icon style="color: red;" icon="fa-ban"></ons-icon>
		</button>
		
	<iframe name="sendPhoto" style="width:0px;height:0px;border:0px;"></iframe>
	</body>
</html>