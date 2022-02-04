var rover_lat=-1.0;
		var rover_lng=-1.0;
		var RoverLatlng;
		var target_lat=-1.0;
		var target_lng=-1.0;
		var targetLatlng;
		
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

			getRoverLatlng();
			//位置情報取得待ち
			while(rover_lat==-1.0&&rover_lng==-1.0){
					const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
					await _sleep(100);
			}

			console.log( "中央座標\n[" + rover_lat + "," + rover_lng + "]\nでmap表示" ) ;

			map = new google.maps.Map(document.getElementById("map"), {
				center: { lat: rover_lat, lng: rover_lng},
				zoom: 17
			});
			//マーカーの初期プロパティを指定
			 rover_marker = new google.maps.Marker({
			  position: RoverLatlng,
			  map: map,
			  icon: {
				url: '../pic/panda.png',                      //アイコンのURL
				scaledSize: new google.maps.Size(40, 40) //サイズ
			},
			//optimized: false             //★写真を丸くするために「false」を指定する
			 });
			 target_marker = new google.maps.Marker({
			  position: targetLatlng,
			  map: map
			 });
			 
			rover_marker.setMap(map);

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

		function　mapSentorToRover(){
			map.panTo(RoverLatlng);
		}
		function setMarker(lat_lng, map) {
			// //マーカー削除
			//   MarkerClear();

			console.log("lat:"+lat_lng.lat()+"\nlng:"+lat_lng.lng()+"\n");


			// マーカーを設置
			var marker = new google.maps.Marker({
				position: lat_lng,
				map: map
			});

			// 座標の中心をaずらす
			// http://syncer.jp/google-maps-javascript-api-matome/map/method/panTo/
			map.panTo(lat_lng);
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
			 
			});
			RoverLatlng = new google.maps.LatLng(rover_lat, rover_lng);
			//var rover_Latlng = new google.maps.LatLng(lat,lng);
			//setMarker(rover_Latlng, map);
