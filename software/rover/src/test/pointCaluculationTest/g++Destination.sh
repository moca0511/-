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

# vincenty
cd ${libDir}/pointCalculation/
g++ -c vincenty.cpp 
ar -r libvincenty.a vincenty.o 

echo startMain
cd ${nowDir}
g++ vincenty_destination.cpp\
	-lvincenty -I${libDir}	-L${libDir}/pointCalculation	\
	-o destination.out

./destination.out  35.4681 133.0486 21.21518366 490.58216516

<<COMMENT
  POINT-1:   35.46810000°,  133.04860000°
AZIMUTH-1:       21.21518366°
   LENGTH:      490.58216516m
-->
  POINT-2:   35.47222200°,  133.05055600°
AZIMUTH-2:      201.21631869°


COMMENT
