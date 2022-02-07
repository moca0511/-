#!/bin/bash
a=$(ls -d */)

# ディレクトリ内の.mkファイルに一括で変更を加える

for i in $a; do
	#file=$i/lib2.mk
	cp ./jsonPost/sample/Makefile $i/sample
	#sed -i s/-include ../-include $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../ ${file}
	#echo "" >> ${file}
	#echo -include ../../root.mk >>$file
	#sed -i -e '$d' ${file}
	#sed -i -e '$d' ${file}
	#sed -i '1i @$(make) -e -f ../../root.mk' ${file}
	#sed -i -e '1d'  ${file}
done
