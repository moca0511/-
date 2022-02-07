#!/bin/bash

cd "$(dirname "$0")" || exit

PS="$(ps aux | grep ./tcp_client.py | grep -v grep)"

if [ -n "${PS}" ]; then
	#	echo "$PS"
	echo -n ""
else
	cd relay || exit
	sh start_reray.sh &
	#sh start_reray.sh >/tmp/relay.log &
	cd ..
fi


./transfer_fromServer_ToRoverConsole/transferConsole.out &
./transfer_fromRoverToServer/transferRead.out    &

sleep 1 

