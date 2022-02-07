#include "guide.h"
#include <string.h>
#include "pointCalculation/vincenty.h"

void init_lon_lat(LON_LAT *data)
{
	init_struct_LON_LAT_POINT(&data->start);
	init_struct_distination(&data->now);
	init_struct_distination(&data->target);
	data->rtkResult = RECORD_NULL;
}

void init_struct_distination(DISTINATION_STRUCT *data)
{
	memset(data, 0, sizeof(DISTINATION_STRUCT));
	data->recodType = RECORD_NULL;
}

int MyGuide::setGuideTarget(const double nowDirection, const LON_LAT data)
{
	std::tuple<double, double, double> t;
	switch (data.target.recodType)
	{
	case RECORD_END_LON_LAT:
	case RECORD_END_DISTINATION_ABSOLUTE:
	case RECORD_END_DISTINATION_RELATIVE:
		break;
	default:
		printf("err : %s recordType=%d", __func__, data.target.recodType);
		return -1;
	}

	guideLonLat = data;
	guideLonLat.start = guideLonLat.now.point;
	my_lib::Vincenty v_cal(guideLonLat.start.lat, guideLonLat.start.lon);
	if (data.target.recodType == RECORD_END_LON_LAT) //目標地点が記録されているので、目標方位と距離を計算
	{
		t = v_cal.calc_distance(guideLonLat.target.point.lat, guideLonLat.target.point.lon);
		guideLonLat.target.distination.m = std::get<0>(t);
		guideLonLat.target.distination.direction = std::get<1>(t);
	}
	else
	{
		double targetDirection = guideLonLat.target.distination.direction;
		if (data.target.recodType == RECORD_END_DISTINATION_RELATIVE)
		{
			if (nowDirection == -1)
			{
				printf("%s(%s)方位データのエラー\n", __FILE__, __func__);
				exit(1);
			}
			targetDirection = fmod(targetDirection + nowDirection + 360.0, 360.0);
		}
		//目標距離と方位が記録されているので、目標地点を計算
		t = v_cal.calc_destination(targetDirection, guideLonLat.target.distination.m);
		guideLonLat.target.point.lat = std::get<0>(t);
		guideLonLat.target.point.lon = std::get<1>(t);
	}
	guideLonLat.target.recodType = RECORD_RESULT;

	return 0;
}

/*
戻り値1：0=正常終了、-1=現在の位置情報が間違っている、-2=方位センサーの値がおかしい、
戻り値2：現在角度と、現在位置から目標位置までの角度の差
*/
std::tuple<int, double> MyGuide::calNow(const double nowDirection, const LON_LAT data)
{
	double differenceDirection = 0;
	if (data.now.recodType == RECORD_END_LON_LAT)
	{
		//現在座標とターゲット座標から必要な距離と角度を会得
		std::tuple<double, double, double> t;
		guideLonLat.now.point = data.now.point;
		my_lib::Vincenty v_cal(guideLonLat.now.point.lat, guideLonLat.now.point.lon);
		t = v_cal.calc_distance(guideLonLat.target.point.lat, guideLonLat.target.point.lon);
		guideLonLat.now.distination.m = std::get<0>(t);
		guideLonLat.now.distination.direction = std::get<1>(t);
		guideLonLat.now.recodType = RECORD_RESULT;

		if (nowDirection == -1)
		{
			printf("err:%s(%s[%d])\n", __FILE__, __func__, __LINE__);
			return std::forward_as_tuple(-2, 0);
		}
		differenceDirection = getDifferenceDirection(guideLonLat.now.distination.direction, nowDirection);
	}
	else
	{
		printf("err:%s(%s[%d])\n", __FILE__, __func__, __LINE__);
		return std::forward_as_tuple(-1, 0);
	}
	return std::forward_as_tuple(0, differenceDirection);
}
