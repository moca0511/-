/***********************************************************
  Vincenty 法で、楕円体上の2点間の距離、地点 1, 2 における
  方位角を計算

    DATE          AUTHOR          VERSION
    2020.09.13    mk-mode.com     1.00 新規作成

  Copyright(C) 2020 mk-mode.com All Rights Reserved.
***********************************************************/
#include "pointCalculation/vincenty.h"

#include <cstdlib>   // for EXIT_XXXX
#include <iomanip>   // for setprecision
#include <iostream>
#include <string>
#include <tuple>

int main(int argc, char* argv[]) {
  double lat_1;  // 地点 1 緯度
  double lng_1;  // 地点 1 経度
  double lat_2;  // 地点 2 緯度
  double lng_2;  // 地点 2 経度
  std::tuple<double, double, double> t;  // 1-2 の距離, 1, 2 における方位角

  try {
    // コマンドライン引数のチェック
    if (argc < 5) {
      std::cerr << "[ERROR] Number of arguments is wrong!\n"
                << "[USAGE] ./vincenty_distance lat_1 long_1 lat_2 long_2"
                << std::endl;
      return EXIT_FAILURE;
    }

    // 地点 1, 2 の緯度・経度取得
    lat_1 = std::stod(argv[1]);
    lng_1 = std::stod(argv[2]);
    lat_2 = std::stod(argv[3]);
    lng_2 = std::stod(argv[4]);
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "  POINT-1: "
              << std::setw(13) << std::right << lat_1 << "°, "
              << std::setw(13) << std::right << lng_1 << "°"
              << std::endl;
    std::cout << "  POINT-2: "
              << std::setw(13) << std::right << lat_2 << "°, "
              << std::setw(13) << std::right << lng_2 << "°"
              << std::endl;

    // 計算
    my_lib::Vincenty v(lat_1, lng_1);
    t = v.calc_distance(lat_2, lng_2);

    // 結果出力
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "-->" << std::endl;
    std::cout << "   LENGTH: "
              << std::setw(17) << std::right << std::get<0>(t)
              << std::endl;
    std::cout << "AZIMUTH-1: "
              << std::setw(17) << std::right << std::get<1>(t)
              << std::endl;
    std::cout << "AZIMUTH-2: "
              << std::setw(17) << std::right << std::get<2>(t)
              << std::endl;
  } catch (...) {
      std::cerr << "EXCEPTION!" << std::endl;
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
