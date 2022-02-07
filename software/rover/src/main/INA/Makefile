make = make --no-print-directory

DIR_PATH_ROVER = ${ROOT_PATH}src/main/rover
DIR_PATH_MAIN = ${ROOT_PATH}src/main/main_thread

# Makefile_header
export FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
export SOURCE=main.cpp
export SOURCE_SUB=
export PROGRAM = main.out
export LIB_USE=liblogFile.a  libINA260.a libpastTime.a
export E_LIB=-lwiringPi  # 配布ライブラリ

# プライマリターゲット
.PHONY: all
all: make

echo:
	$(make) -e -C ${ROOT_PATH}src -f main_master.mk echo

# プログラムの生成ルール
make:
	$(make) -e -C ${ROOT_PATH}src -f main_master.mk

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(make) -e -C ${ROOT_PATH}src -f main_master.mk clean

run : make
	sudo -E ${FILE_DIR}${PROGRAM}



-include $(FILE_DIR)../../../root.mk
