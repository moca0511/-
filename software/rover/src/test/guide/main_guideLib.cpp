#include "guide/guide.h"

#include "guide/guideSupport.h"

int main()
{
	LON_LAT nowLonLat;
	MyGuide guide;
	double nowDirection = -1;
	char temp_str[500];
	init_lon_lat(&nowLonLat);

	nowLonLat.now.point.lon = 133.0486;
	nowLonLat.now.point.lat = 35.4681;
	nowLonLat.now.recodType = RECORD_END_LON_LAT;

	nowLonLat.target.point.lat = 35.472222;
	nowLonLat.target.point.lon = 133.050556;
	nowLonLat.target.recodType = RECORD_END_LON_LAT;

	nowLonLat.target.distination.direction = 21.21518366;
	nowLonLat.target.distination.m = 490.58216516;
	nowLonLat.target.recodType = RECORD_END_DISTINATION_ABSOLUTE;

	printf("%d\n", nowLonLat.target.recodType);

	guide.setGuideTarget(-1,nowLonLat);

	nowLonLat = guide.get();
	sprintf(temp_str, "緯度(lat, y)・経度(lon, x), n(lat,lon)=(%lf, %lf), s(lat,lon)=(%lf, %lf), e(lat,lon)=(%lf, %lf), s->e(m, dir)=(%lf, %lf),",
			nowLonLat.now.point.lat, nowLonLat.now.point.lon,
			nowLonLat.start.lat, nowLonLat.start.lon,
			nowLonLat.target.point.lat, nowLonLat.target.point.lon,
			nowLonLat.target.distination.m, nowLonLat.target.distination.direction);
	printf("%s", temp_str);
	printf("\n");

	printf("\n");

	for (int i = 0; i < 3; i++)
	{
		std::tuple<int, double> r;
		nowLonLat.now.point.lat = getDoubleRand_LON_LAT(35.4681);
		nowLonLat.now.point.lon = getDoubleRand_LON_LAT(133.0486);
		nowLonLat.now.recodType = RECORD_END_LON_LAT;
		nowDirection = getDoubleRand_direction(180);
		//nowDirection = -1;

		r = guide.calNow(nowDirection, nowLonLat);
		if (std::get<0>(r) == 0)
		{
			nowLonLat = guide.get();
			sprintf(temp_str, "緯度(lat, y)・経度(lon, x), n(lat,lon)=(%lf, %lf), s(lat,lon)=(%lf, %lf), e(lat,lon)=(%lf, %lf), s->e(m, dir)=(%lf, %lf), n->e(m, dir)=(%lf, %lf), dir(now, diffrence)=(%f, %f)",
					nowLonLat.now.point.lat, nowLonLat.now.point.lon,
					nowLonLat.start.lat, nowLonLat.start.lon,
					nowLonLat.target.point.lat, nowLonLat.target.point.lon,
					nowLonLat.target.distination.m, nowLonLat.target.distination.direction,
					nowLonLat.now.distination.m, nowLonLat.now.distination.direction,
					nowDirection, std::get<1>(r));
			printf("%s", temp_str);
			printf("\n");
		}
		else
		{
			printf("false guide.call_Now\n");
		}
	}
	return 0;
}
