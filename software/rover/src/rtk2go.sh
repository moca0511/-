#!/bin/bash

sleep 3
AAA=$(curl http://rtk2go.com:2101 | grep -v grep | grep RAKU-BASE)

echo "${AAA}"


if [ -n "${AAA}" ]; then
    /usr/bin/str2str -in ntrip://rtk2go.com:2101/RAKU-BASE -out serial://ttyACM0:115200#2103
else
#    /usr/bin/str2str -in serial://ttyACM0:115200 -out tcpsvr://:2103
    /usr/bin/str2str -in ntrip://rtk2go.com:2101/RAKU-BASE -out serial://ttyACM0:115200#2103
fi
