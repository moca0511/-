#!/bin/bash

# source ./addPath_str2str.sh
echo ${PATH}
. ./str2str.path
export PATH=${PATH}:${STR2SSTR_PATH}
echo ${PATH}
#str2str
