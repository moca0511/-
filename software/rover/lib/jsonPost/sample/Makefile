#------------------------------
# 全体用変数
#------------------------------
make = make --no-print-directory
SOURCE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))../

#------------------------------
# sample用変数
#------------------------------
CC=g++
CFLAGS = -Wno-psabi -lcurl 
LIB=libjsonPost.a

SAMPLE_OUT[1]=sample-1.out
SAMPLE_OUT[2]=sample-2.out
SAMPLE_OUT[3]=sample-3.out
SAMPLE_OUT[4]=sample-4.out
SAMPLE_OUT[5]=sample-5.out

SAMPLE_SOURCE[1]=main_send.cpp
SAMPLE_SOURCE[2]=main_str2num.cpp
SAMPLE_SOURCE[3]=main_num2str.cpp
SAMPLE_SOURCE[4]=main_totalJson.cpp
SAMPLE_SOURCE[5]=main_totalJson2.cpp

LIB_DIR=${ROOT_PATH}lib
LIBS=${LIB:lib%.a=-l%}

CC-OPTION=-I${LIB_DIR} -L${LIB_LIB_DIR} $(CFLAGS) ${LIBS}
#------------------------------


all: allSample

allSample:
	@$(make) -e sample-1
	@$(make) -e sample-2
	@$(make) -e sample-3
	@$(make) -e sample-4
	@$(make) -e sample-5

${SAMPLE_OUT[1]}:${SAMPLE_SOURCE[1]}
	@$(eval A = g++1)
	$(CC) -o ${SAMPLE_OUT[1]} ${SAMPLE_SOURCE[1]} $(CC-OPTION)
	
${SAMPLE_OUT[2]}:${SAMPLE_SOURCE[2]}
	@$(eval A = g++2)
	$(CC) -o ${SAMPLE_OUT[2]} ${SAMPLE_SOURCE[2]} $(CC-OPTION)
	
${SAMPLE_OUT[3]}:${SAMPLE_SOURCE[3]}
	@$(eval A = g++3)
	$(CC) -o ${SAMPLE_OUT[3]} ${SAMPLE_SOURCE[3]} $(CC-OPTION)
	
${SAMPLE_OUT[4]}:${SAMPLE_SOURCE[4]}
	@$(eval A = g++4)
	$(CC) -o ${SAMPLE_OUT[4]} ${SAMPLE_SOURCE[4]} $(CC-OPTION)

${SAMPLE_OUT[5]}:${SAMPLE_SOURCE[5]}
	@$(eval A = g++5)
	$(CC) -o ${SAMPLE_OUT[5]} ${SAMPLE_SOURCE[5]} $(CC-OPTION)
	
sample-1: makeLib ${SAMPLE_OUT[1]} makeCall
sample-2: makeLib ${SAMPLE_OUT[2]} makeCall
sample-3: makeLib ${SAMPLE_OUT[3]} makeCall
sample-4: makeLib ${SAMPLE_OUT[4]} makeCall
sample-5: makeLib ${SAMPLE_OUT[5]} makeCall

# run-%をtabキーで表示するために記述
run-1:
run-2:

run-%:sample-%
	@echo run '${@:run-%=sample-%.out}'
	@./'${@:run-%=sample-%.out}'

makeLib:
	@$(make) -f $(SOURCE_DIR)lib2.mk

makeCall:
	@echo "make out $(A)"
	@echo

echo:
	@echo ${TARGET_LIB}
	@echo ${LIB_DIR}
	@echo ${SOURCE_DIR}
	@echo ${SOURCES}
	@echo $(HEADER)
	@echo ${OBJS}
	@echo lib_master.mk
	@$(make) -e -f $(SOURCE_DIR)lib2.mk echo

clean: 
	@$(make) -e -f $(SOURCE_DIR)lib2.mk clean
	@rm *.out -f

-include $(SOURCE_DIR)../../root.mk
