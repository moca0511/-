make = make --no-print-directory

DIR_PATH_ROVER = ${ROOT_PATH}src/main/rover
DIR_PATH_MAIN = ${ROOT_PATH}src/main/main

# プライマリターゲット
.PHONY: all
all: make

echo:
	$(make) -e -C ${DIR_PATH_ROVER} -f rover.mk echo
	$(make) -e -C ${DIR_PATH_MAIN} -f main.mk echo

# プログラムの生成ルール
make:
	$(make) -e -C ${DIR_PATH_ROVER} -f rover.mk
	$(make) -e -C ${DIR_PATH_MAIN} -f main.mk


# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(make) -e -C ${DIR_PATH_ROVER} -f rover.mk clean
	$(make) -e -C ${DIR_PATH_MAIN} -f main.mk clean


run : make
	sudo -E ${FILE_DIR}${PROGRAM}

-include $(FILE_DIR)../../../root.mk
