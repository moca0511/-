CC=g++
make = make --no-print-directory
export LIB=libpico_quick.a
export SOURCES=pico_quick.cpp
export CFLAGS = 
export ONLY_HEADERS=

SAMPLE_OUT[1]=sample-1.out

SAMPLE_SOURCE[1]=test.cpp

export SOURCE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))../
LIB_DIR=${ROOT_PATH}lib
LIBS=${LIB:lib%.a=-l%}

all: clean allSample

allSample:
	@$(make) -e sample-1

${SAMPLE_OUT[1]}:${SAMPLE_SOURCE[1]}
	@$(eval A = g++1)
	$(CC) -o ${SAMPLE_OUT[1]} ${SAMPLE_SOURCE[1]} -I${LIB_DIR} -L${LIB_LIB_DIR} $(CFLAGS) ${LIBS}

sample-1: make ${SAMPLE_OUT[1]} makeCall

make:
	@echo make $(LIB)
	@$(make) -e -f ${LIB_DIR}/lib_master.mk

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
	@$(make) -e -f ${LIB_DIR}/lib_master.mk echo

clean: 
	@$(make) -e -f ${LIB_DIR}/lib_master.mk clean
	@rm *.out -f

-include $(SOURCE_DIR)../../root.mk
