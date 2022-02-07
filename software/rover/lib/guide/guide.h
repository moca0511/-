#ifndef GUIDE_H
#define GUIDE_H

#include "tcpServerClient/tcpStruct.h"
#include "pointCalculation/cal_direction.h"
#include <tuple>
#define DISTINATION_STRUCT msg4

#define RTK_RESULT__NULL 0
#define RTK_RESULT__SINGLE 1
#define RTK_RESULT__FIX 4
#define RTK_RESULT__FLOT 5

typedef struct
{
	LON_LAT_POINT start;	   //スタート座標
	DISTINATION_STRUCT now;	   //現在座標と、目標への距離と方向
	DISTINATION_STRUCT target; //目標座標と、スタート座標から目標座標までの距離と方向
	int rtkResult;			   //現在座標のRTK法の結果
} LON_LAT;

void init_lon_lat(LON_LAT *data);
void init_struct_distination(DISTINATION_STRUCT *data);

class MyGuide
{
private:
	LON_LAT guideLonLat;
	double nowDirection;

public:
	//	LON_LAT setGuideTarget(const int type, const int nowRtkResult, const DISTINATION_STRUCT now, const DISTINATION target)
	int setGuideTarget(const double nowDirection, const LON_LAT data);
	std::tuple<int, double> calNow(const double nowDirection, const LON_LAT data);
	LON_LAT get() { return guideLonLat; };
};

#endif
