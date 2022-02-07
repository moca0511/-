make = make --no-print-directory
# Makefile_header
export FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
export SOURCE=main_inaLog.cpp
export PROGRAM = inaLog.out
export LIB_USE=libINA260.a liblogFile.a
export E_LIB= -lwiringPi# 配布ライブラリ

# プライマリターゲット
.PHONY: all
all: make

echo:
	$(make) -e -C ${PRO4_ROVER_DIR_PATH}src  -f main_master.mk echo

# プログラムの生成ルール
make:
	$(make) -e -C ${PRO4_ROVER_DIR_PATH}src  -f main_master.mk

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(make) -e -C ${PRO4_ROVER_DIR_PATH}src  -f main_master.mk clean

run: make
	sudo -E ${FILE_DIR}${PROGRAM}

-include $(FILE_DIR)../../../root.mk
