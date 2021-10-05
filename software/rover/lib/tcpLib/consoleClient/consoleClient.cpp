#include "consoleClient.h"

void consoleTcpClient::str2vector(const char data[])
{
	std::string str, s;
	str = data;
	std::stringstream ss{str};
	while (getline(ss, s, ' '))
		v.push_back(s);
	vectorSize = v.size();
	return;
}

void consoleTcpClient::vectorClear()
{
	for (int i = 0; i < vectorSize; i++)
		v.pop_back();
}

std::string consoleTcpClient::pop(int num)
{
	return v[num];
}

void consoleTcpClient::printConsoleSupport(int dataType)
{
	switch (dataType)
	{
	case 0:
	case 1:
	case 2:
		printf("use only 'convert count' (dataType=%d)\n", dataType);
		break;
	case 3:
		printf("moterL, MoterR, time, queueAction\n");
		break;
	case 4:
		printf("type, end Lat, end Lon, km, direction, queueAction \n");
		break;
	case 5:
		printf("moterOperation, time, queueAction\n");
		break;
	default:
		printf("err : %s dataType\n", __func__);
		break;
	}
}

void consoleTcpClient::char2tcpStruct(tcpStruct *data, const char str_in[])
{
	str2vector(str_in);
	data->buf[0] = '\0';

	if (data->dataType == 0)
	{
		data->useData.data0.num1 = convertCount;
	}
	else if (data->dataType == 1)
	{
		data->useData.data1.num2 = convertCount * 1.1;
	}
	else if (data->dataType == 2)
	{
		data->useData.data2.num3 = convertCount;
	}
	else if (data->dataType == 3)
	{
		if (vCount() != 4)
		{
			printf("strSize err : type=%d, count=%d[%s]\n", data->dataType, vCount(), str_in);
			exit(1);
		}
		data->useData.data3.moterL = std::stoi(pop(0));
		data->useData.data3.moterR = std::stoi(pop(1));
		data->useData.data3.time = std::stoi(pop(2));
		data->queueAction = std::stoi(pop(3));
	}
	else if (data->dataType == 4)
	{
		if (vCount() != 6)
		{
			printf("strSize err : type=%d, count=%d[%s]\n", data->dataType, vCount(), str_in);
			exit(1);
		}
		data->useData.data4.sendType = std::stoi(pop(0));
		data->useData.data4.end.lat = std::stod(pop(1));
		data->useData.data4.end.lon = std::stod(pop(2));
		data->useData.data4.distination.km = std::stod(pop(3));
		data->useData.data4.distination.direction = std::stod(pop(4));

		data->queueAction = std::stoi(pop(5));
	}
	else if (data->dataType == 5)
	{
		if (vCount() != 3)
		{
			printf("strSize err : type=%d, count=%d[%s]\n", data->dataType, vCount(), str_in);
			exit(1);
		}
		data->useData.data5.moterOperation = std::stoi(pop(0));
		data->useData.data5.time = std::stoi(pop(1));
		data->queueAction = std::stoi(pop(2));
	}
	else
	{
		printf("err : %s dataType\n", __func__);
	}
	convertCount++;
	vectorClear();
}

std::string consoleTcpClient::tcpStruct2string(tcpStruct *data)
{
	std::string outStr;
	char temp_str[CLIENT_BUF_SIZE];
	switch (data->dataType)
	{
	case 0:
		sprintf(temp_str, "%d", data->useData.data0.num1);
		break;
	case 1:
		sprintf(temp_str, "%d", data->useData.data1.num2);
		break;
	case 2:
		sprintf(temp_str, "%d", data->useData.data2.num3);
		break;
	case 3:
		sprintf(temp_str, "(%3d, %3d)%dms [queue=%d]", data->useData.data3.moterL, data->useData.data3.moterR, data->useData.data3.time, data->queueAction);
		break;
	case 4:
		sprintf(temp_str, "sendType=%d, n(lat,lon)=(%lf, %lf), s(lat,lon)=(%lf, %lf), e(lat,lon)=(%lf, %lf), (km, direction)=(%lf, %lf), [queue=%d]",
				data->useData.data4.sendType,
				data->useData.data4.now.lat, data->useData.data4.now.lon,
				data->useData.data4.start.lat, data->useData.data4.start.lon,
				data->useData.data4.end.lat, data->useData.data4.end.lon,
				data->useData.data4.distination.km, data->useData.data4.distination.direction,
				data->queueAction);
		break;
	case 5:
		sprintf(temp_str, "mOperateion=%d, time=%d, [queue=%d]",
				data->useData.data5.moterOperation,
				data->useData.data5.time,
				data->queueAction);
		break;
	default:
		printf("err : %s:  dataTpe\n", __func__);
		temp_str[0] = '\0';
		break;
	}
	outStr = temp_str;
	return outStr;
}
