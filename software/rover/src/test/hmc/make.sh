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

# hmc
cd ${libDir}/hmc6352/
g++ -c hmc.cpp  -lwiringPi
ar -r libhmc.a hmc.o 

echo startMain
cd ${nowDir}
g++ main.cpp\
	-lhmc -I${libDir}	-L${libDir}/hmc6352  -lwiringPi	\
	-o sample.out
