#!/bin/bash
BLUETOOTH_ADDR=50:02:91:A1:58:86

# connect
#echo "connect ${BLUETOOTH_ADDR} \nquit" | bluetoothctl

#bind

if [ ! -e /dev/rfcomm1 ]; then
	sudo rfcomm bind 1 ${BLUETOOTH_ADDR}
fi

# add your program below

: <<COMMENT_OUT
bluetoothctl

agent on
default-agent
discoverable on
scan on

pair addr

https://ossyaritoori.hatenablog.com/entry/2019/01/11/M5stack%E3%81%A8Raspberry_Pi_zero%E3%81%AEBluetooth_%E3%82%B7%E3%83%AA%E3%82%A2%E3%83%AB%E9%80%9A%E4%BF%A1
COMMENT_OUT
