#!/bin/sh
# see also http://mizti.hatenablog.com/entry/2013/01/27/204343
#include<unistd.h>
usage() {
  echo "実行するには2個の引数が必要です。
  第一引数: 監視対象ファイル名
  第二引数: 監視対象ファイルが更新された際に実行されるコマンド
  例： ./autoexec.sh a.cpp 'g++ a.cpp && ./a.cpp'"
}
update() {
  echo `openssl sha256 -r $1 | awk '{print $1}'`
}
if [ $# -ne 2 ];
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
mode=0
while true;
do
  sleepenh $INTERVAL > 0
  current=`update $1`
  if [ "$last" != "$current" ];
  then
    line_log=$line
    line=`cat $1 | wc -l`
    nowdate=`date '+%Y/%m/%d'`
    nowtime=`date '+%H:%M:%S'`
    echo "no:$no\tdate:$nowdate\ttime:$nowtime\tfile:$1\tline:$line"
    eval $2
    last=$current
    no=`expr $no + 1`
#最新コマンド抽出
    update_line=$((line-line_log))
    for i in `seq 1 $((update_line))`
    do
	    echo "$i"
	    cat /var/www/html/log/log.csv | head -n $((line-update_line+i)) | tail -n 1
#コマンド解析　POST    
    done
  fi  
done
