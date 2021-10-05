nowDir=$('pwd')
echo ${nowDir}

if [ "${PRO4_ROVER_DIR_PATH:+foo}" ];then
	echo ${PRO4_ROVER_DIR_PATH}
else 
	echo PRO4_ROVER_DIR_PATHがない
	exit 1
fi

libDir=${PRO4_ROVER_DIR_PATH}/lib/tcpLib
echo ${libDir}

# tcpStruct
cd ${libDir}/common
g++ -c tcpStruct.cpp

cd ${libDir}/myServer/
g++ -c tcpServer.cpp  -L${libDir} -I${libDir} 
ar -r libServer.a tcpServer.o ../common/tcpStruct.o

cd ${nowDir}
g++ main_roverServer.cpp -lServer -o serverTcp.out  \
			-I${libDir} \
			-L${libDir}/myServer
