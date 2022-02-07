#!/bin/bash

cd "$(dirname $0)" || exit

TMP_FILE="/tmp/str_recode.tmp"
BASE_URL_FILE="/tmp/baseURL.tmp"

rm "$BASE_URL_FILE"
rm "$TMP_FILE"
