#!/bin/bash

cd "$(dirname "$0")" || exit


cd main || exit

cd f9p || exit
sh str2str.sh >/dev/null 2>&1 &
cd .. || exit

