#!/bin/bash
cd "$(dirname "$0")" || exit
NUM=0
ttyUbox=$(cat ubox.path)

while true; do
	NUM=$((NUM + 1))
	echo conect="${NUM}"
	nc -l 2102  < /dev/"${ttyUbox}"  > /dev/"${ttyUbox}"
done
