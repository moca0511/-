#!/bin/bash

# ディレクトリ内の.mkファイルに一括で変更を加える

find ./lib -name "*.mk" -type f | while read -r fname; do
	echo "$fname"
	sed -i s@'-include ../../../root.mk'@' '@ ${fname}
	sed -i s@'-include ../../root.mk'@' '@ ${fname}
done

#find ./ -name "Makefile" -type f | while read -r fname; do
#	echo "$fname"
#	sed -i s/'-include ..'/'-include $(FILE_DIR)..'/ ${fname}
#done
