nowDir=$('pwd')
echo ${nowDir}

if [ "${PRO4_ROVER_DIR_PATH:+foo}" ];then
	echo ${PRO4_ROVER_DIR_PATH}
else 
	echo PRO4_ROVER_DIR_PATHがない
	exit 1
fi

libDir=${PRO4_ROVER_DIR_PATH}/lib
echo ${libDir}


# tcpStruct
cd ${libDir}/tcpLib/common
g++ -c tcpStruct.cpp

# myTcpClient
cd ${libDir}/tcpLib/myClient/
g++ -c tcpClient.cpp  -L${libDir}/tcpLib -I${libDir}/tcpLib 
ar -r libClient.a tcpClient.o  ../common/tcpStruct.o

# myMoter
cd ${libDir}/moter/obj
gcc -c ../pwmLAP.cpp
ar rcs libmoterLAP.a pwmLAP.o

# pastTime
cd ${libDir}/pastTime/
g++ -c pastTime.cpp 
ar -r libpastTime.a pastTime.o 

cd ${libDir}/f9p/
g++ -c f9p_tcp.cpp 
ar -r libf9p.a f9p_tcp.o 

# log
cd ${libDir}/logFile/
g++ -c myLog.cpp  
ar -r liblog.a myLog.o 

# vincenty && cal_direction
cd ${libDir}/pointCalculation/
g++ -c vincenty.cpp cal_direction.cpp
ar -r libvincenty.a vincenty.o cal_direction.o

# hmc
cd ${libDir}/hmc6352/
g++ -c hmc.cpp  -lwiringPi
ar -r libhmc.a hmc.o 

echo startMain

cd ${nowDir}
g++ main.cpp\
	-lClient -I${libDir}	-L${libDir}/tcpLib/myClient\
	-lwiringPi \
	-lmoterLAP -L${libDir}/moter/obj	\
	-lpastTime -I${libDir}	-L${libDir}/pastTime	\
	-lf9p -I${libDir}	-L${libDir}/f9p	\
	-lvincenty -I${libDir}	-L${libDir}/pointCalculation	\
	-lhmc -I${libDir}	-L${libDir}/hmc6352  -lwiringPi	\
	-llog -I${libDir}	-L${libDir}/logFile  \
	-o rover.out 

#	-std=c++11	\
