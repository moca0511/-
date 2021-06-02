
cd `dirname $0`
g++ motor2.cpp -lmoterLAP -lwiringPi -L../lib/obj -I../lib -o ../bin/sample2.out
