#include "vincenty.h"

namespace my_lib
{
	// 定数
	constexpr double kA = 6378137.0;				 // GRS80 長半径
	constexpr double kF = 1.0 / 298.257222101;		 // GRS80 扁平率
	constexpr double kPi = ((std::atan(1.0)) * 4.0); // PI	//((std::atan(1.0)) * 4.0);
	constexpr double kPi2 = kPi * 2.0;				 // PI * 2
	constexpr double kPi180 = kPi / 180.0;			 // PI / 180
	constexpr double kPi180Inv = 1.0 / kPi180;		 // 1 / (PI / 180)
	constexpr double kEps = 1.0e-11;				 // 1^(-11) = 10^(-12) で 0.06mm の精度

	Vincenty::Vincenty(double lat_1, double lng_1)
	{
		try
		{
			b = kA * (1.0 - kF);
			phi_1 = deg2rad(lat_1);
			l_1 = deg2rad(lng_1);
			tan_u_1 = (1.0 - kF) * std::tan(phi_1);
			u_1 = std::atan(tan_u_1);
		}
		catch (...)
		{
			throw;
		}
	}

	void Vincenty::set(double lat_1, double lng_1)
	{
		try
		{
			b = kA * (1.0 - kF);
			phi_1 = deg2rad(lat_1);
			l_1 = deg2rad(lng_1);
			tan_u_1 = (1.0 - kF) * std::tan(phi_1);
			u_1 = std::atan(tan_u_1);
		}
		catch (...)
		{
			throw;
		}
	}

	/**
   * @brief      地点 2 の緯度・経度、地点 2 における方位を計算
   *             （Vincenty 順解法）
   *
   * @param[in]  az_1: 地点 1 における方位角(double)
   * @param[in]     s: 地点 1〜2 の距離(double)
   * @return     {
   *               lat_2: 地点 2 の緯度(double),
   *               lng_2: 地点 2 の経度(double),
   *                az_2: 地点 2 における方位角(double)
   *             }(tuple)
   */
	std::tuple<double, double, double> Vincenty::calc_destination(double az_1, double s)
	{
		double lat_2 = 0.0; // 地点 2 の緯度
		double lng_2 = 0.0; // 地点 2 の経度
		double az_2 = 0.0;	// 地点 2 における方位角
		double alp_1;
		double cos_alp_1; // cos(α_1)
		double sin_alp_1; // sin(α_1)
		double sgm_1;
		double sin_alp;	 // sin(α)
		double sin2_alp; // sin(α)^2
		double cos2_alp; // cos(α)^2 = 1 - sin(α)^2
		double u2;
		double aa; // A
		double bb; // B
		double cc; // C
		double sgm;
		double sgm_prev;
		double cos_sgm; // cos(σ)
		double sin_sgm; // sin(σ)
		double cos_2_sgm_m;
		double d_sgm;	// Δσ
		double cos_u_1; // cos(u_1)
		double sin_u_1; // sin(u_1)
		double cu1_cs;	// cos(u_1) * cos(σ)
		double cu1_ss;	// cos(u_1) * sin(σ)
		double su1_cs;	// sin(u_1) * cos(σ)
		double su1_ss;	// sin(u_1) * sin(σ)
		double tmp;
		double phi_2;
		double lmd;
		double l;
		double l_2;
		double alp_2;

		try
		{
			alp_1 = deg2rad(az_1);
			cos_alp_1 = std::cos(alp_1);
			sin_alp_1 = std::sin(alp_1);
			sgm_1 = std::atan2(tan_u_1, cos_alp_1);
			sin_alp = std::cos(u_1) * std::sin(alp_1);
			sin2_alp = sin_alp * sin_alp;
			cos2_alp = 1.0 - sin2_alp;
			u2 = cos2_alp * (kA * kA - b * b) / (b * b);
			aa = calc_a(u2);
			bb = calc_b(u2);
			sgm = s / (b * aa);
			sgm_prev = kPi2;

			while (std::abs(sgm - sgm_prev) > kEps)
			{
				cos_sgm = std::cos(sgm);
				sin_sgm = std::sin(sgm);
				cos_2_sgm_m = std::cos(2.0 * sgm_1 + sgm);
				d_sgm = calc_dlt_sgm(bb, cos_sgm, sin_sgm, cos_2_sgm_m);
				sgm_prev = sgm;
				sgm = s / (b * aa) + d_sgm;
			}

			cos_u_1 = std::cos(u_1);
			sin_u_1 = std::sin(u_1);
			cu1_cs = cos_u_1 * cos_sgm;
			cu1_ss = cos_u_1 * sin_sgm;
			su1_cs = sin_u_1 * cos_sgm;
			su1_ss = sin_u_1 * sin_sgm;
			tmp = su1_ss - cu1_cs * cos_alp_1;
			phi_2 = std::atan2(
				su1_cs + cu1_ss * cos_alp_1,
				(1.0 - kF) * std::sqrt(sin_alp * sin_alp + tmp * tmp));
			lmd = std::atan2(sin_sgm * sin_alp_1, cu1_cs - su1_ss * cos_alp_1);
			cc = calc_c(cos2_alp);
			l = lmd - (1.0 - cc) * kF * sin_alp * (sgm + cc * sin_sgm * (cos_2_sgm_m + cc * cos_sgm * (-1.0 + 2.0 * cos_2_sgm_m * cos_2_sgm_m)));
			l_2 = l + l_1;
			alp_2 = std::atan2(sin_alp, -su1_ss + cu1_cs * cos_alp_1) + kPi;
			lat_2 = rad2deg(phi_2);
			lng_2 = rad2deg(norm_lng(l_2));
			az_2 = rad2deg(norm_az(alp_2));

			return {lat_2, lng_2, az_2};
		}
		catch (...)
		{
			throw;
		}

		return {s, az_1, az_2}; // 計算成功
	}

	/**
   * @brief      距離と方位角 1, 2 を計算
   *             （Vincenty 逆解法）
   *
   * @param[in]  lat_2: 緯度(double)
   * @param[in]  lng_2: 経度(double)
   * @return     {
   *                  s: 距離と方位角1,2を計算(double),
   *               az_1: 地点 1 における方位角(double),
   *               az_2: 地点 2 における方位角(double)
   *             }(tuple)
   */
	std::tuple<double, double, double> Vincenty::calc_distance(double lat_2, double lng_2)
	{
		double s = 0.0;	   // 地点 1 と 2 の距離
		double az_1 = 0.0; // 地点 1 における方位角
		double az_2 = 0.0; // 地点 2 における方位角
		double phi_2;
		double l_2;
		double u_2;
		double cos_u_1; // cos(u_1)
		double cos_u_2; // cos(u_2)
		double sin_u_1; // sin(u_1)
		double sin_u_2; // sin(u_2)
		double su1_su2; // sin(u_1) * sin(u_2)
		double su1_cu2; // sin(u_1) * cos(u_2)
		double cu1_su2; // cos(u_1) * sin(u_2)
		double cu1_cu2; // cos(u_1) * cos(u_2)
		double l;
		double lmd;
		double lmd_prev;
		double cos_lmd;
		double sin_lmd;
		double t_0;
		double t_1;
		double sin_sgm;
		double cos_sgm;
		double sgm;
		double sin_alp;
		double cos2_alp;
		double cos_2_sgm_m;
		double aa; // A
		double bb; // B
		double cc; // C
		double u2;
		double d_sgm; // Δσ
		double alp_1;
		double alp_2;

		try
		{
			phi_2 = deg2rad(lat_2);
			l_2 = deg2rad(lng_2);
			u_2 = std::atan((1.0 - kF) * std::tan(phi_2));
			cos_u_1 = std::cos(u_1);
			cos_u_2 = std::cos(u_2);
			sin_u_1 = std::sin(u_1);
			sin_u_2 = std::sin(u_2);
			su1_su2 = sin_u_1 * sin_u_2;
			su1_cu2 = sin_u_1 * cos_u_2;
			cu1_su2 = cos_u_1 * sin_u_2;
			cu1_cu2 = cos_u_1 * cos_u_2;
			l = norm_lng(l_2 - l_1);
			lmd = l;
			lmd_prev = kPi2;
			cos_lmd = std::cos(lmd);
			sin_lmd = std::sin(lmd);

			while (std::abs(lmd - lmd_prev) > kEps)
			{
				t_0 = cos_u_2 * sin_lmd;
				t_1 = cu1_su2 - su1_cu2 * cos_lmd;
				sin_sgm = std::sqrt(t_0 * t_0 + t_1 * t_1);
				cos_sgm = su1_su2 + cu1_cu2 * cos_lmd;
				sgm = std::atan2(sin_sgm, cos_sgm);
				sin_alp = cu1_cu2 * sin_lmd / sin_sgm;
				cos2_alp = 1 - sin_alp * sin_alp;
				cos_2_sgm_m = cos_sgm - 2 * su1_su2 / cos2_alp;
				cc = calc_c(cos2_alp);
				lmd_prev = lmd;
				lmd = l + (1.0 - cc) * kF * sin_alp * (sgm + cc * sin_sgm * (cos_2_sgm_m + cc * cos_sgm * (-1.0 + 2.0 * cos_2_sgm_m * cos_2_sgm_m)));
				cos_lmd = std::cos(lmd);
				sin_lmd = std::sin(lmd);
				if (lmd > kPi)
				{
					lmd = kPi;
					break;
				}
			}

			u2 = cos2_alp * (kA * kA - b * b) / (b * b);
			aa = calc_a(u2);
			bb = calc_b(u2);
			d_sgm = calc_dlt_sgm(bb, cos_sgm, sin_sgm, cos_2_sgm_m);
			s = b * aa * (sgm - d_sgm);
			alp_1 = std::atan2(cos_u_2 * sin_lmd, cu1_su2 - su1_cu2 * cos_lmd);
			alp_2 = std::atan2(cos_u_1 * sin_lmd, -su1_cu2 + cu1_su2 * cos_lmd) + kPi;
			az_1 = rad2deg(norm_az(alp_1));
			az_2 = rad2deg(norm_az(alp_2));

			return {s, az_1, az_2};
		}
		catch (...)
		{
			throw;
		}

		return {s, az_1, az_2}; // 計算成功
	}

	/**
   * @brief      度 => ラジアン
   *
   * @param[in]  deg: 度      (double)
   * @return     rad: ラジアン(double)
   */
	double Vincenty::deg2rad(double deg)
	{
		try
		{
			return deg * kPi180;
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * @brief      ラジアン => 度
   *
   * @param[in]  rad: ラジアン(double)
   * @return     deg: 度      (double)
   */
	double Vincenty::rad2deg(double rad)
	{
		try
		{
			return rad * kPi180Inv;
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * @brief  A 計算
   *
   * @param[in] u2: u^2 の値
   * @return     a: A の値(double)
   */
	double Vincenty::calc_a(double u2)
	{
		try
		{
			return 1.0 + u2 / 16384.0 * (4096.0 + u2 * (-768.0 + u2 * (320.0 - 175.0 * u2)));
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * @brief  B 計算
   *
   * @param[in] u2: u^2 の値
   * @return     b: B の値(double)
   */
	double Vincenty::calc_b(double u2)
	{
		try
		{
			return u2 / 1024.0 * (256.0 + u2 * (-128.0 + u2 * (74.0 - 47.0 * u2)));
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * @brief  C 計算
   *
   * @param[in] cos2_alp: cos^2(α) の値
   * @return           c: C の値(double)
   */
	double Vincenty::calc_c(double cos2_alp)
	{
		try
		{
			return kF * cos2_alp * (4.0 + kF * (4.0 - 3.0 * cos2_alp)) / 16.0;
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * Δσ 計算
   *
   * @param[in]          bb: B の値
   * @param[in]     cos_sgm: cos(σ) の値
   * @param[in]     sin_sgm: sin(σ) の値
   * @param[in] cos_2_sgm_m: cos(2σ_m) の値
   * @return        dlt_sgm: Δσ の値
   */
	double Vincenty::calc_dlt_sgm(
		double bb, double cos_sgm, double sin_sgm, double cos_2_sgm_m)
	{
		try
		{
			return bb * sin_sgm * (cos_2_sgm_m + bb / 4.0 * (cos_sgm * (-1.0 + 2.0 * cos_2_sgm_m * cos_2_sgm_m) - bb / 6.0 * cos_2_sgm_m * (-3.0 + 4.0 * sin_sgm * sin_sgm) * (-3.0 + 4.0 * cos_2_sgm_m * cos_2_sgm_m)));
		}
		catch (...)
		{
			return 0.0;
		}
	}

	/**
   * @brief      経度正規化
   *
   * @param[in]  lng: 正規化前の経度(rad)(double)
   * @return     lng: 正規化後の経度(rad)(double)
   */
	double Vincenty::norm_lng(double lng)
	{
		try
		{
			while (lng < -kPi)
				lng += kPi2;
			while (lng > kPi)
				lng -= kPi2;
		}
		catch (...)
		{
			return 0.0;
		}

		return lng;
	}

	/*
   * 方位角正規化
   *
   *  @param[in]  alp: 正規化前の方位角(rad)
   *  @return     alp: 正規化後の方位角(rad)
   */
	double Vincenty::norm_az(double alp)
	{
		try
		{
			if (alp < 0.0)
				alp += kPi2;
			if (alp > kPi2)
				alp -= kPi2;
		}
		catch (...)
		{
			return 0.0;
		}

		return alp;
	}
}
