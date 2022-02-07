#!/bin/bash

cd "$(dirname $0)" || exit

BASE_URL_FILE="/tmp/baseURL.tmp"
CMD="$(cat ${BASE_URL_FILE})"
#echo "$(python3 str2str.py "$CMD")"
python3 str2str.py "$CMD" > /tmp/str2str.py.log
