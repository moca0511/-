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

# f9pTcp
cd ${libDir}/f9p/
g++ -c f9p_tcp.cpp 
ar -r libf9p.a f9p_tcp.o 

echo startMain
cd ${nowDir}
g++ main_f9p_tcp.cpp\
	-lf9p -I${libDir}	-L${libDir}/f9p	\
	-o f9p.out
