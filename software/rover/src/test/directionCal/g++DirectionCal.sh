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

# vincenty && cal_direction
cd ${libDir}/pointCalculation/
g++ -c vincenty.cpp cal_direction.cpp
ar -r libvincenty.a vincenty.o cal_direction.o

echo startMain
cd ${nowDir}
g++ main_directionCal.cpp\
	-lvincenty -I${libDir}	-L${libDir}/pointCalculation	\
	-o direction.out
