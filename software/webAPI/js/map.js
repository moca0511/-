const KCG_RAKUHOKU_LAT = 35.04423053007073;
const KCG_RAKUHOKU_LNG = 135.7722187666102;


var my_lat=0.0;
var my_lng=0.0;

function getMyLatlng(){
			
// Geolocation APIに対応している
	if( navigator.geolocation )
	{
		console.log("gpsOK\n" ) ;
		// 現在地を取得
		navigator.geolocation.getCurrentPosition(

			// [第1引数] 取得に成功した場合の関数
			function( position )
			{
				// 取得したデータの整理
				var data = position.coords ;

				// データの整理
				var lat = data.latitude ;
				var lng = data.longitude ;
				var alt = data.altitude ;
				var accLatlng = data.accuracy ;
				var accAlt = data.altitudeAccuracy ;
				var heading = data.heading ;			//0=北,90=東,180=南,270=西
				var speed = data.speed ;

				// アラート表示
				console.log( "成功現在位置は、\n[" + lat + "," + lng + "]\nです。" ) ;

				// 位置情報
				my_lat=lat;
				my_lng=lng;
			},

			// [第2引数] 取得に失敗した場合の関数
			function( error )
			{
				// エラーコード(error.code)の番号
				// 0:UNKNOWN_ERROR				原因不明のエラー
				// 1:PERMISSION_DENIED			利用者が位置情報の取得を許可しなかった
				// 2:POSITION_UNAVAILABLE		電波状況などで位置情報が取得できなかった
				// 3:TIMEOUT					位置情報の取得に時間がかかり過ぎた…

				// エラー番号に対応したメッセージ
				var errorInfo = [
					"原因不明のエラーが発生しました…。" ,
					"位置情報の取得が許可されませんでした…。" ,
					"電波状況などで位置情報が取得できませんでした…。" ,
					"位置情報の取得に時間がかかり過ぎてタイムアウトしました…。"
				] ;

				// エラー番号
				var errorNo = error.code ;

				// エラーメッセージ
				var errorMessage = "[エラー番号: " + errorNo + "]\n" + errorInfo[ errorNo ] ;

				// アラート表示
				alert( errorMessage ) ;

				// // HTMLに書き出し
				// document.getElementById("result").innerHTML = errorMessage;
				// 現在位置を取得できない場合の処理
				// アラート表示
				
				my_lat=KCG_RAKUHOKU_LAT;
				my_lng=KCG_RAKUHOKU_LNG;
				console.log( "失敗位置は、\n[" + my_lat + "," + my_lng + "]\nです。" ) ;
			} ,

			// [第3引数] オプション
			{
				"enableHighAccuracy": true,
				"timeout": 5000,
				"maximumAge": 2000,
			}

		) ;
		}

		// Geolocation APIに対応していない
		else
		{
			// 現在位置を取得できない場合の処理
			// アラート表示
				console.log( "未取得現在位置は、\n[" + lat + "," + lng + "]\nです。" ) ;
			my_lat=KCG_RAKUHOKU_LAT;
			my_lng=KCG_RAKUHOKU_LNG;
		}

					//GPS取得タイムアウト待ち
		
	}

	async function waitMs(wait_time){
			const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
			await _sleep(wait_time);
	}

async function initMap() {

	getMyLatlng();

	//位置情報取得待ち
	while(my_lat==0.0&&my_lng==0.0){
			const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));
			await _sleep(100);
	}

	console.log( "中央座標\n[" + my_lat + "," + my_lng + "]\nでmap表示" ) ;

	var map = new google.maps.Map(document.getElementById("map"), {
		center: { lat: my_lat, lng: my_lng},
		zoom: 17
	});

	// クリックイベントを追加
	map.addListener('click', function(e) {
		setMarker(e.latLng, map);
	});

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