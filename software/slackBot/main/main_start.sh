#!/bin/bash
cd "$(dirname $0)" || exit

cd slackbot || exit
echo "引数の数 $#"
if [ "$#" = 2 ]; then
	echo "sleep $2"
	sleep "$2"
fi
python3 run.py "$1"
