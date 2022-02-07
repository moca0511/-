#!/bin/bash
# 引数のpythonをkillするプログラム
# killしたいプログラムは.pyまで含む

cd "$(dirname "$0")" || exit

if [ "$#" != 1 ]; then
	return
	sleep 
fi

PS="$(ps aux | grep "$1" | grep -v grep)"
# echo "$PS"

for pid in $(pgrep '^python3$'); do
	AAA="$(echo "$PS" | grep "$pid")"
	if [ -n "${AAA}" ]; then
		kill "$pid"
		PS=
	fi
	AAA=
done

sleep 1 
