make = make --no-print-directory
# Makefile_header
export FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
export SOURCE=rover.cpp
export SOURCE_SUB=
export PROGRAM = rover.out
export LIB_USE=libf9p.a libmoter.a libpastTime.a libtcpServerClient.a libguide.a liblogFile.a libpointCalculation.a libmyFork.a libmyQueue.a libpico_quick.a
#export LIB_USE=libf9p.a libhmc.a libmoter.a libpastTime.a libtcpServerClient.a libguide.a liblogFile.a libpointCalculation.a libmyFork.a libmyQueue.a libpico_quick.a
export E_LIB=-lwiringPi # 配布ライブラリ

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
