make = make --no-print-directory
# Makefile_header
export FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
export SOURCE=main.cpp
export SOURCE_SUB=backgroundFunc.cpp
export PROGRAM = main.out
export LIB_USE=libtcpServerClient.a liblogFile.a libpointCalculation.a libmyFork.a libbluetooth_m5stikC.a libINA260.a libpastTime.a
export E_LIB=-lwiringPi  -pthread# 配布ライブラリ

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
