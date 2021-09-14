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

cd ${libDir}/tcpLib/myClient/
g++ -c tcpClient.cpp  -L${libDir}/tcpLib -I${libDir}/tcpLib 
ar -r libClient.a tcpClient.o 

cd ${nowDir}

g++ main_myMonitorTcp.cpp -lClient -o myMonitorTcp.out  \
			-I${libDir}	\
			-L${libDir}/tcpLib/myClient
