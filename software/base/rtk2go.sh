#!/bin/bash

sleep $1

ps=`ps aux | grep str2str | grep -v grep | wc -l`
if [ $ps -eq 0 ]; then # 対象プロセスが存在しない場合
	echo "rtk2go start"
	/usr/local/bin/str2str -in serial://ttyUbox9 -out ntrips://:9bZSdVdK@rtk2go.com:2101/RAKU-BASE 
	echo "process none"
	echo "exit 0"
	exit 0
else
	#echo 1 >> /home/pi/rtk2go.log
	echo "using str2str"
fi
