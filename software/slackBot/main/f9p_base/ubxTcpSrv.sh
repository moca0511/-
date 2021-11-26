#!/bin/bash
cd "$(dirname "$0")" || exit
ttyUbox=$(cat ubox.path)

/usr/local/bin/str2str -in serial://"${ttyUbox}" -out tcpsvr:://:2102
