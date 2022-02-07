#!/bin/bash

cd "$(dirname $0)" || exit
# shellcheck disable=SC1091
. ./str2str.path


export PATH=${PATH}:${STR2SSTR_PATH}
echo "${BASE_ADDRESS}"
str2str -in tcpcli://"${BASE_ADDRESS}":2101 -out serial://ttyACM0:115200#2103  
str2str -in tcpcli://"${BASE_ADDRESS}":2101 -out serial://ttyACM0:115200#2103  



