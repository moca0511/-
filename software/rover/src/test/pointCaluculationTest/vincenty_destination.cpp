/***********************************************************
  Vincenty 法で、1点から指定の方位角・距離にある点を求める

    DATE          AUTHOR          VERSION
    2020.09.15    mk-mode.com     1.00 新規作成

  Copyright(C) 2020 mk-mode.com All Rights Reserved.
***********************************************************/
#include "pointCalculation/vincenty.hpp"

#include <cstdlib> // for EXIT_XXXX
#include <iomanip> // for setprecision
#include <iostream>
#include <string>
#include <tuple>

int main(int argc, char *argv[])
{
	double lat_1; // 地点 1 緯度
	double lng_1; // 地点 1 経度
	double az_1;  // 地点 1 における方位角
	double s;	  // 地点 1〜2 の距離
	std::tuple<double, double, double> t;
	// 地点 2 の緯度・経度、地点 2 における方位角

	try
	{
		// コマンドライン引数のチェック
		if (argc < 5)
		{
			std::cerr << "[ERROR] Number of arguments is wrong!\n"
					  << "[USAGE] ./vincenty_destination lat_1 lng_1 az_1 s"
					  << std::endl;
			return EXIT_FAILURE;
		}

		// 地点 1, 2 の緯度・経度、地点 1 における方位角、地点 1〜2 の距離取得
		lat_1 = std::stod(argv[1]);
		lng_1 = std::stod(argv[2]);
		az_1 = std::stod(argv[3]);
		s = std::stod(argv[4]);
		std::cout << std::fixed << std::setprecision(8);
		std::cout << "  POINT-1: "
				  << std::setw(13) << std::right << lat_1 << "°, "
				  << std::setw(13) << std::right << lng_1 << "°"
				  << std::endl;
		std::cout << "AZIMUTH-1: "
				  << std::setw(17) << std::right << az_1 << "°"
				  << std::endl;
		std::cout << "   LENGTH: "
				  << std::setw(17) << std::right << s << "m"
				  << std::endl;

		// 計算
		my_lib::Vincenty v(lat_1, lng_1);
		t = v.calc_destination(az_1, s);

		// 結果出力
		std::cout << std::fixed << std::setprecision(8);
		std::cout << "-->" << std::endl;
		std::cout << "  POINT-2: "
				  << std::setw(13) << std::right << std::get<0>(t) << "°, "
				  << std::setw(13) << std::right << std::get<1>(t) << "°"
				  << std::endl;
		std::cout << "AZIMUTH-2: "
				  << std::setw(17) << std::right << std::get<2>(t) << "°"
				  << std::endl;
	}
	catch (...)
	{
		std::cerr << "EXCEPTION!" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
