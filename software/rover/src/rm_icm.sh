#!/bin/bash

cd "$(dirname "$0")" || exit

PS="$(ps aux | grep ./icm.py | grep -v grep)"
 echo "$PS"

for pid in $(pgrep '^python3$'); do
	AAA="$(echo "$PS" | grep "$pid")"
	if [ -n "${AAA}" ]; then
		kill "$pid"
		PS=
	fi
	AAA=
done

sleep 1 
