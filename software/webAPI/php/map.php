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
		#map {
			height: 100%;
			width: 100%;
			float : right;
			font-size:200%;
			text-align: center;
			display: flex;
			align-items: center;
			justify-content: center
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

	</head>
	<body>
		
			
			
			<div id="map">
				<div>
					<ons-icon icon="spinner" spin="true" size="2x"></ons-icon>
					<p>Location information acquisition</p>
				</div>
			</div>
			
		

	<script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDy_HecQGmuV8HTbl_z3j6cJ7OCOMfaw0o&callback=initMap" async defer></script>
	</body>
</html>