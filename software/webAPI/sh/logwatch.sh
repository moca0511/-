#!/bin/sh
# see also http://mizti.hatenablog.com/entry/2013/01/27/204343
#include<unistd.h>
usage() {
  echo "実行するには2個の引数が必要です。
  第一引数: 監視対象ファイル名
  例： ./autoexec.sh a.cpp"
}
update() {
  echo `openssl sha256 -r $1 | awk '{print $1}'`
}
if [ $# -ne 1 ];
then
  usage
  exit 1
fi

INTERVAL=0.01 #監視間隔, 秒で指定
no=0
last=`update $1`
line=`cat $1 | wc -l`
line_log=`cat $1 | wc -l`
update_line=0
mode_str="stop"
mode_num=0
update="0"
lat=0.0
lng=0.0
file_str="0,0.0,0.0"
while true;
do
  sleepenh $INTERVAL > /dev/null
  current=`update $1`
  if [ "$last" != "$current" ];
  then
    line_log=$line
    line=`cat $1 | wc -l`
    nowdate=`date '+%Y/%m/%d'`
    nowtime=`date '+%H:%M:%S'`
    echo "\n"
    echo "no:$no\tdate:$nowdate\ttime:$nowtime\tfile:$1\tline:$line"

    last=$current
    no=`expr $no + 1`
#最新コマンド抽出
    update_line=$((line-line_log))
    for i in `seq 1 $((update_line))`
    do
	    update=`cat /var/www/html/log/log.csv | head -n $((line-update_line+i)) | tail -n 1`
#コマンド解析　POST
		mode_num=`echo $update | cut -d, -f3`
		lat=`echo $update | cut -d, -f6`
		lng=`echo $update | cut -d, -f7`
		file_str="$mode_num $lat $lng"
		echo "$file_str"
		
		#echo "$file_str" > /home/rakuhoku/move_command.txt
		echo "{\"mode\": "$mode_num",\"lat\": "$lat",\"lng\": "$lng"}" > /home/rakuhoku/move_command.fifo
		#python3 /var/www/html/python/createJson.py $mode_num $lat $lng /home/rakuhoku/move_command.json
		
		
	done
	echo "\n"
  fi  
done
