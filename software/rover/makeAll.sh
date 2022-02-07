#!/bin/bash

echo src/main 内のMakefile実行開始

#find  -exec make -s -f  {}  > /dev/null \;
find ./src/main -name "Makefile" -type f | while read -r fname; do
	echo "$fname"
	make -s -f "$fname" >/dev/null
done

echo " "

echo src/sub 内のMakefile実行開始
#find ./src/sub -name "Makefile"  -type f -exec make -s -f  {} > /dev/null \;

find ./src/sub -name "Makefile" -type f | while read -r fname; do
	echo "$fname"
	make -s -f "$fname" >/dev/null
done

echo " "

echo lib/*/sample 内のMakefile実行開始
nowD=$(pwd)
find ./lib/*/sample -name "Makefile" -type f | while read -r fname; do
	echo "$fname"
	path=$(dirname "$fname")
	cd "${path}" || exit
	make >/dev/null
	cd "$nowD" || exit
done
