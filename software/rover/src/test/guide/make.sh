#!/bin/bash

cd `dirname $0` || exit
nowDir=$('pwd')
echo "$nowDir"

if [ "${PRO4_ROVER_DIR_PATH:+foo}" ];then
	echo "$PRO4_ROVER_DIR_PATH"
else 
	echo PRO4_ROVER_DIR_PATHがない
	exit 1
fi

libDir="$PRO4_ROVER_DIR_PATH"/lib
echo "$libDir"

# tcpStruct
cd "$libDir"/tcpServerClient || exit
g++ -c tcpStruct.cpp

# myTcpClient
cd "$libDir"/tcpServerClient || exit
g++ -c tcpClient.cpp  -L"$libDir"/tcpServerClient -I"$libDir" 
ar -r libClient.a tcpClient.o  tcpStruct.o

# pastTime
cd "$libDir"/pastTime/ || exit
g++ -c pastTime.cpp 
ar -r libpastTime.a pastTime.o 

cd "$libDir"/f9p/ || exit
g++ -c f9p_tcp.cpp 
ar -r libf9p.a f9p_tcp.o 

# log
cd "$libDir"/logFile/ || exit
g++ -c myLog.cpp  
ar -r liblog.a myLog.o 

# vincenty && cal_direction
cd "$libDir"/pointCalculation/  || exit
g++ -c vincenty.cpp cal_direction.cpp 	-I"$libDir" 
ar -r libvincenty.a vincenty.o cal_direction.o

cd "$libDir/moter/" || exit
g++ -c pwmLAP.cpp roverMoter.cpp -I"$libDir"
ar -r libmoter.a pwmLAP.o roverMoter.o

# guide
cd "$libDir"/guide || exit
g++ -c guide.cpp guideSupport.cpp	-I"$libDir" 
ar -r libguide.a	guide.o guideSupport.o

echo startMain

cd "$nowDir" || exit
g++ main_guideLib.cpp \
	-I"$libDir"	\
	-lguide 	-L"$libDir"/guide	\
	-lClient 	-L"$libDir"/tcpServerClient\
	-lwiringPi \
	-lpastTime	-L"$libDir"/pastTime	\
	-lf9p 		-L"$libDir"/f9p	\
	-lvincenty 	-L"$libDir"/pointCalculation	\
	-llog 		-L"$libDir"/logFile  \
	-lmoter		-L"$libDir"/moter\
	-o guideLib.out 

#	-std=c++11	\

echo ""
