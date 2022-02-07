#!/bin/bash

cd "$(dirname "$0")" || exit



GPIO=21    #使用するGPIOポート

#初期設定
sudo sh -c "echo $GPIO >/sys/class/gpio/export"
sudo sh -c "echo in >/sys/class/gpio/gpio$GPIO/direction"
sudo sh -c "echo high >/sys/class/gpio/gpio$GPIO/direction"


# 起動時間測定
start_time=`date +%s`


END_FLG=0
COUNT=0
LONG_LOW=0

### 計測したい処理・コマンド ###
while :
do
    end_time=`date +%s`
    run_time=$((end_time - start_time))
    if [ "${run_time}" -gt 10 ];then
        data=$(cat /sys/class/gpio/gpio$GPIO/value)
        if [ "$data" -eq "0" ]; then
            COUNT=$(($COUNT+1))
	    echo countUp
            echo "${COUNT}"
        else
		COUNT=0
        fi
        # LOWを3s以上続けた
        if [ "${COUNT}" -gt 3 ];then
            LONG_LOW=1
	    echo longLow
        fi
        
        if [ "$LONG_LOW" -eq 1 ]; then
            if [ "$data" -eq "1" ]; then
                # LOWを3s以上続けた後にHIGHになった
                echo LOW-HIGH
		END_FLG=1
		break
            fi
        fi
        sleep 1
    fi
done




sudo sh -c "echo $GPIO >/sys/class/gpio/unexport"

if [ "${END_FLG}" -eq 1 ];then
    sudo shutdown -h now
    echo shutdown
fi
