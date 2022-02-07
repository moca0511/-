#!/bin/bash

cd "$(dirname $0)" || exit

str2str -in serial://ttyACM0:115200 -out tcpsvr://:2101



