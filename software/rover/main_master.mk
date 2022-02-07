

# プログラム名とオブジェクトファイル名
#PROGRAM = ${FILE_DIR}main.out
#LIB_USE=libf9p.a    libhmc.a      libmoter.a   libpastTime.a	libtcpServerClient.a	liblogFile.a  libpointCalculation.a
#E_LIB=-lwiringPi # 配布ライブラリ
#FILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
#SOURCE=${FILE_DIR}main.cpp

PROGRAM_PATH=${FILE_DIR}${PROGRAM}
SOURCE_PATH=${SOURCE:%=${FILE_DIR}%}
HEADER_PATH=${SOURCE_PATH:%.cpp=%.h}

LIBS_PATH=${LIB_USE:%=${LIB_LIB_DIR}/%}
LIBS=${LIB_USE:lib%.a=-l%}
LIB_DIR_NAME=${LIB_USE:lib%.a=%}

# 定義済みマクロの再定義
CC = g++

# プライマリターゲット
.PHONY: all
all: depend $(PROGRAM_PATH)

echo:
	@echo program"		": ${PROGRAM_PATH}
	@echo include dir"	": ${INCLUDE_DIR}
	@echo LibLib dir"	": ${LIB_LIB_DIR}
	@echo now dir"		": ${FILE_DIR}
	@echo ${SOURCE_PATH} ${HEADER_PATH}
	@echo ${LIB_DIR_NAME}
	@echo ${LIBS}

# プログラムの生成ルール
$(PROGRAM_PATH): $(SOURCE_PATH) ${LIBS_PATH}
	$(CC) -o $(PROGRAM_PATH) $(SOURCE_PATH) -I${INCLUDE_DIR} -L${LIB_LIB_DIR}	${LIBS} ${E_LIB}

${LIBS_PATH}:make_lib
${LIB_DIR_NAME}:

make_lib : ${LIB_DIR_NAME}
	@echo ライブラリの作成開始
	@for i in ${LIB_DIR_NAME}; do	\
		echo $$i ;\
		$(make) -C	${LIB_DIR}/$${i} -f lib2.mk;	\
	done
	@echo ライブラリの作成の終了
	@echo " "

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(RM) -f $(PROGRAM_PATH)  ${FILE_DIR}depend.inc
	@for i in ${LIB_DIR_NAME}; do	\
		echo $$i ;\
		$(make) -e -C	${LIB_DIR}/$${i} -f lib2.mk clean;	\
	done

# ヘッダファイルの依存関係
.PHONY: depend
depend: $(SOURCE_PATH)
	-@ $(RM) ${FILE_DIR}depend.inc
	-@ for i in $^; do cpp -MM $$i -I${INCLUDE_DIR} | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> ${FILE_DIR}depend.inc; done

-include depend.inc

