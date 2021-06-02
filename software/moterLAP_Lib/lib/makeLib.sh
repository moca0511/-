cd `dirname $0`
cd obj
gcc -c ../encoder.cpp
gcc -c ../pwmLAP.cpp
ar rcs libmoterLAP.a encoder.o pwmLAP.o
