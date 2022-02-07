#ifndef VINCENTY_CALC_DESTINATION_VINCENTY_HPP_
#define VINCENTY_CALC_DESTINATION_VINCENTY_HPP_

#include <tuple>

#include <cmath>
#include <iostream>
#include <tuple>

//方位角は北を基点に時計回り

namespace my_lib
{
	class Vincenty
	{
		double b;											 // 極半径（短半径）
		double phi_1;										 // 地点 1 の緯度
		double l_1;											 // 地点 1 の経度
		double tan_u_1;										 // tan(地点 1 の更成緯度)
		double u_1;											 // 地点 1 の更成緯度
		double deg2rad(double);								 // 度 => ラジアン
		double rad2deg(double);								 // ラジアン => 度
		double calc_a(double);								 // A 計算
		double calc_b(double);								 // B 計算
		double calc_c(double);								 // C 計算
		double calc_dlt_sgm(double, double, double, double); // Δσ 計算
		double norm_lng(double);							 // 経度正規化
		double norm_az(double);								 // 方位角正規化

	public:
		Vincenty(double, double); // コンストラクタ
		void set(double, double);	//地点1の経度、緯度を設定
		std::tuple<double, double, double> calc_destination(double, double);
		// 地点 2 の緯度(lat)・経度(lon)、地点 2 における方位を計算	// （Vincenty 順解法）

		std::tuple<double, double, double> calc_distance(double, double);
		// 距離と方位角 1, 2 を計算		//（Vincenty 逆解法）
	};
}

#endif
