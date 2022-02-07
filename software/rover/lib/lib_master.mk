PRDUCTION_DIR=${ROOT_PATH}obj

CC=g++
#CFLAGS = -Wall -O2

INCLUDE_DIR=${LIB_DIR}
LIB_LIB_DIR=${PRDUCTION_DIR}/lib
LIB_DIR_NAME=${LIB:lib%.a=%}
DIPEND_DIR=${PRDUCTION_DIR}/depend
OBJS_DIR=${PRDUCTION_DIR}/obj

TARGET_LIB=${LIB:%=${LIB_LIB_DIR}/%}
OBJS=${SOURCES:%.cpp=${OBJS_DIR}/%.o}
#OBJS=$(addprefix ${OBJS_DIR}/,$(notdir $(SOURCES:.cpp=.o)))
HEADER=${SOURCES:%.cpp=%.h} ${ONLY_HEADERS}
SOURCES_PATH=${SOURCES:%=${SOURCE_DIR}%}
HEADER_PATH=${HEADER:%=${SOURCE_DIR}%}

all: $(TARGET_LIB) depend

echo:
	@echo AAA
	@echo libxxx.a"	": ${TARGET_LIB}
	@echo include dir"	": ${INCLUDE_DIR}
	@echo LibLib dir"	": ${LIB_LIB_DIR}
	@echo now dir"		": ${SOURCE_DIR}
	@echo 元ファイル"	"${SOURCES} ${HEADER}
	@echo 元ソースパス"	"${SOURCES_PATH}
	@echo 元ヘッダパス"	"${HEADER_PATH}
	@echo ${LIB_DIR_NAME}
	@echo *.oファイル"	"${OBJS}
	@echo ${SOURCE_DIR}${SOURCES}
	@echo AAA
	
# プログラムの生成ルール
$(TARGET_LIB): $(OBJS) $(HEADER_PATH)
	@echo $(OBJS)
	@ar -r $(TARGET_LIB) $(OBJS)

# サフィックスルール
${OBJS_DIR}/%.o : ${SOURCE_DIR}%.cpp  $(HEADER_PATH)
	@echo $<
	@echo $@
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDE_DIR}

clean: 
	rm -f ${TARGET_LIB} ${OBJS} ${DIPEND_DIR}/depend_${LIB_DIR_NAME}.inc

# ヘッダファイルの依存関係
.PHONY: depend
depend: $(SOURCES_PATH)
	-@ $(RM) ${DIPEND_DIR}/depend_${LIB_DIR_NAME}.inc
	-@ for i in $^; do cpp -MM $$i -I${INCLUDE_DIR} | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> ${DIPEND_DIR}/depend_${LIB_DIR_NAME}.inc; done

#-include ..pend.inc
#-include ../root.mk
