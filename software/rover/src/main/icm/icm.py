from __future__ import print_function
from tkinter.tix import Y_REGION
import qwiic_icm20948
import time
import sys
import math

import os
#https://sukhoi.hatenadiary.com/entry/2019/08/23/194323#Hard-Iron-offset

X_OFFSET = 91  # (X_MAX - XMIN) / 2 - X_MAX
Y_OFFSET = -173.5

LOG_FILE = 'myfile.csv'


def fileOpen():
    if(os.path.exists('/tmp/icm.fifo')==False ):
        os.system("mkfifo /tmp/icm.fifo")
    fifo = open("/tmp/icm.fifo", "w")
    f = open(LOG_FILE, 'a')
    f.write("\nX_OFFSET{:4}".format(X_OFFSET) +
            ", Y_OFFSET{:4}\n".format(Y_OFFSET))
    return fifo, f


def runExample():
    while True:
        fifo, f = fileOpen()
        print("\nSparkFun 9DoF ICM-20948 Sensor  Example 1\n")
        IMU = qwiic_icm20948.QwiicIcm20948()

        if IMU.connected == False:
            print("The Qwiic ICM20948 device isn't connected to the system. Please check your connection", \
            file=sys.stderr)
            return

        IMU.begin()

        X_MAX=0.0
        Y_MAX=0.0
        X_MIN=0.0
        Y_MIN=0.0
        while True:
            if IMU.dataReady():
                IMU.getAgmt(
                )  # read all axis and temp from sensor, note this also updates all instance variables
                AAA = math.degrees(
                    math.atan2(IMU.myRaw + Y_OFFSET, IMU.mxRaw + X_OFFSET))
                if(AAA<0):
                    AAA=-1*AAA
                else:
                    AAA=360-AAA

                if(IMU.mxRaw>X_MAX):
                    X_MAX=IMU.mxRaw
                elif (IMU.mxRaw<X_MIN):
                    X_MIN=IMU.mxRaw

                if(IMU.myRaw>Y_MAX):
                    Y_MAX=IMU.myRaw
                elif (IMU.myRaw<Y_MIN):
                    Y_MIN=IMU.myRaw

                writeData=(\
            # '{: 06d}'.format(IMU.axRaw)\
			#, '\t', '{: 06d}'.format(IMU.ayRaw)\
			#, '\t', '{: 06d}'.format(IMU.azRaw)\
			#, '\t', '{: 06d}'.format(IMU.gxRaw)\
			#, '\t', '{: 06d}'.format(IMU.gyRaw)\
			#, '\t', '{: 06d}'.format(IMU.gzRaw)\
                "{: 06d}, ".format(IMU.mxRaw)+\
                "{: 06d}, ".format(IMU.myRaw)+\
                "{: 06d}, ".format(IMU.mzRaw)+\
                "{: 05.2f}".format(AAA)+\
                "{: 06.2f}, ".format((X_MAX-X_MIN)/2-X_MAX)+\
                "{: 06.2f}, ".format((Y_MAX-Y_MIN)/2-Y_MAX))

                print(str(writeData))

                fifo.write("{: 04.2f},".format(AAA))
                fifo.flush()

                f.write(str(writeData) + "\n")
                time.sleep(0.03)
            else:
                print("Waiting for data")
                time.sleep(0.5)


if __name__ == '__main__':
    while True:
        try:
            runExample()
        except (KeyboardInterrupt, SystemExit) as exErr:
            print("\nEnding Example 1")
            sys.exit(0)
        except Exception as e:
            print(e)
