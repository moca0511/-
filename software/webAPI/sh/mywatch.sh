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
last=$2
while true;
do
  sleepenh $INTERVAL >stderr
  current=`update $1`
  if [ "$last" != "$current" ];
  then
    last=$current
    echo $last
    exit 0
  fi  
done
