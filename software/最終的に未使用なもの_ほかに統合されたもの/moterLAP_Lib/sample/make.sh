
cd `dirname $0`
g++ motor.cpp -lmoterLAP -lwiringPi -L../lib/obj -I../lib -o ../bin/sample.out
