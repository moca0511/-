#!/bin/bash
cd "$(dirname "$0")" || exit

# 終了コマンド
sh main/relay_with_server/rm_transfer.sh

echo START_MAIN

# mainコマンド
echo server
cd main/server/ || exit
chmod a+x ./serverTcp.out
./serverTcp.out >/tmp/serverTcp1.log 2>&1 &
cd ../../

echo START_ICM

PS="$(ps aux | grep ./icm.py | grep -v grep)"

if [ -n "${PS}" ]; then
        #       echo "$PS"
        echo -n ""
else
	mkfifo /tmp/icm.fifo
	python3 main/icm/icm.py > /tmp/icm_log.log 2>&1 &
fi

sleep 1

cd main || exit

cd f9p || exit
#sh str2str.sh >/dev/null 2>&1 &
#sh main/f9p/str2str.sh >/dev/null 2>&1 &
cd .. || exit

ps ax > /tmp/ps.log

echo TRANSFER
cd relay_with_server || exit
#sh start_transfer.sh >/tmp/start_transfer.log 2>&1 &
cd .. || exit

cd m5/ || exit
#./m5.out >/dev/null 2>&1 &
cd .. || exit

sleep 2

ps ax >/tmp/ps2.log

echo START_ROVER
cd rover/ || exit
#rm ./rover.out
#make
sleep 1
sudo -E ./rover.out on on # >/tmp/rover.log 
cd .. || exit

# 終了用コマンド
#sh relay_with_server/rm_transfer.sh
#sh ../rm_icm.sh
#rm /tmp/icm.fifo

#pkill str2str
