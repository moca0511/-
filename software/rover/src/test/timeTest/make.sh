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

# myTcpClient
cd ${libDir}/pastTime/
g++ -c pastTime.cpp 
ar -r libpastTime.a pastTime.o 

echo startMain
cd ${nowDir}
g++ time.cpp\
	-lpastTime -I${libDir}	-L${libDir}/pastTime	\
	-o sample.out
