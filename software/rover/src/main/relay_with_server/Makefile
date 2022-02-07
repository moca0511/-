make = make --no-print-directory


AAA = transfer_fromServer_ToRoverConsole
BBB = transfer_fromRoverToServer/

# プライマリターゲット
.PHONY: all
all: make

echo:
	$(make) -e -C ${AAA}   echo
	$(make) -e -C ${BBB}   echo

# プログラムの生成ルール
make:
	$(make) -e -C ${AAA}  
	$(make) -e -C ${BBB}  


# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(make) -e -C ${AAA}   clean
	$(make) -e -C ${BBB}   clean


run : make
	sudo -E ${FILE_DIR}${PROGRAM}

-include $(FILE_DIR)../../../../root.mk
