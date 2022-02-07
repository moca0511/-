#!/bin/bash
NUM=0
while true; do
	NUM=$((NUM + 1))
	echo conect="${NUM}"
	nc -l 2102  < /dev/ttyACM0  > /dev/ttyACM0 
done
