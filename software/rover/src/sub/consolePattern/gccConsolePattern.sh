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

cd ${libDir}/tcpLib/myClient/
g++ -c tcpClient.cpp  -L${libDir}/tcpLib -I${libDir}/tcpLib 
#ar -r libClient.a tcpClient.o 

cd ${libDir}/tcpLib/consoleClient/
g++ -c consoleClient.cpp -L${libDir}/tcpLib -I${libDir}/tcpLib
ar -r libconsole.a ../myClient/tcpClient.o consoleClient.o ../common/tcpStruct.o

cd ${nowDir}

g++ main_consolePattern.cpp -lconsole -o consolePattern.out  \
			-I${libDir}	\
			-L${libDir}/tcpLib/consoleClient
