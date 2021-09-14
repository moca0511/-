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

# log
cd ${libDir}/logFile/
g++ -c myLog.cpp  
ar -r liblog.a myLog.o 


cd ${nowDir}
g++ main_log.cpp\
	-llog -I${libDir}	-L${libDir}/logFile  \
	-o sample.out
