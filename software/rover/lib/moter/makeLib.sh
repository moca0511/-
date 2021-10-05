cd `dirname $0`
cd obj
gcc -c ../pwmLAP.cpp
ar rcs libmoterLAP.a pwmLAP.o
