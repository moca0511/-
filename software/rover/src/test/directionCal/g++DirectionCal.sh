#!/bin/bash

cd "$(dirname "$0")" || exit
nowDir=$('pwd')
echo "${nowDir}"

if [ "${PRO4_ROVER_DIR_PATH:+foo}" ]; then
	echo "${PRO4_ROVER_DIR_PATH}"
else
	echo PRO4_ROVER_DIR_PATHがない
	exit 1
fi

libDir=${PRO4_ROVER_DIR_PATH}/lib
echo "${libDir}"

# vincenty && cal_direction
cd "${libDir}/pointCalculation/" || exit
g++ -c vincenty.cpp cal_direction.cpp -I"${libDir}"
ar -r libvincenty.a vincenty.o cal_direction.o

cd "${libDir}/moter/" || exit
g++ -c pwmLAP.cpp roverMoter.cpp -I"${libDir}"
ar -r libmoter.a pwmLAP.o roverMoter.o

echo startMain
cd "${nowDir}" || exit
g++ main_directionCal.cpp -lvincenty -lmoter -lwiringPi -I"${libDir}" \
	-L"${libDir}"/pointCalculation -L"${libDir}"/moter -o direction.out
echo ""
