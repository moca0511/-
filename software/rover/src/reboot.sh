#!/bin/sh

# GPIOを確認してGNDに接続しているならファイルに記録を行って、body.shを実行する

cd "$(dirname "$0")" || exit


GPIO=21    #使用するGPIOポート

#初期設定
sudo sh -c "echo $GPIO >/sys/class/gpio/export"
sudo sh -c "echo in >/sys/class/gpio/gpio$GPIO/direction"
sudo sh -c "echo high >/sys/class/gpio/gpio$GPIO/direction"

data=$(cat /sys/class/gpio/gpio$GPIO/value)
echo "$data" >> /tmp/reboot_rover_log.log
if [ "$data" -eq "0" ]; then

	if [ "$#" = 1 ]; then
        echo "sleep $1"
        sleep "$1"
	fi

	echo "1" > /tmp/reboot_GPIO.tmp

	echo 'START'
	#sh str2str.sh
	sleep 3
	#sh body.sh
	#pkill str2str
fi

#sudo sh -c "echo $GPIO >/sys/class/gpio/unexport"
