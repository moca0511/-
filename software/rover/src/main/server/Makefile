make = make --no-print-directory
# Makefile_header
export FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
export SOURCE=main_roverServer.cpp
export PROGRAM = serverTcp.out
export LIB_USE=libtcpServerClient.a
export E_LIB=-lwiringPi # 配布ライブラリ

# プライマリターゲット
.PHONY: all
all: make

echo:
	$(make) -e -C ${ROOT_PATH}src  -f main_master.mk echo

# プログラムの生成ルール
make:
	$(make) -e -C ${ROOT_PATH}src  -f main_master.mk

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(make) -e -C ${ROOT_PATH}src  -f main_master.mk clean

-include $(FILE_DIR)../../../root.mk
