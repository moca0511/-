#!/bin/bash

cd "$(dirname "$0")" || exit

PS="$(ps aux | grep ./tcp_client.py | grep -v grep)"

if [ -n "${PS}" ]; then
	#	echo "$PS"
	echo -n ""
else
	cd relay || exit
	sh start_reray.sh >/tmp/relay.log &
	cd ..
fi

sleep 1
#./transfer_fromServer_ToRoverConsole/transferConsole.out >/tmp/transferConsole.tmp &
#./transfer_fromRoverToServer/transferRead.out >/tmp/transferRead.tmp &
./transfer/transfer.out >/tmp/transfer.tmp 2>&1 &

