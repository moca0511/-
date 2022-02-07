#!/bin/bash

cd "$(dirname $0)" || exit
TMP_FILE="/tmp/str_recode.tmp"
BASE_URL_FILE="/tmp/baseURL.tmp"

echo "$(python3 recode_baseURL.py)" > "${TMP_FILE}"
sed -n 2P "$TMP_FILE" > "$BASE_URL_FILE"

