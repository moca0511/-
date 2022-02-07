#!/bin/bash

# source ./addPath_str2str.sh
echo "${PATH}"
# shellcheck disable=SC1091
. "${PRO4_ROVER_DIR_PATH}"/src/main/f9p/str2str.path
export PATH=${PATH}:${STR2SSTR_PATH}
echo "${PATH}"
#str2str
