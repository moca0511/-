make = make --no-print-directory
LIB_DIR=${ROOT_PATH}/lib
export LIB=libmyFork.a
export SOURCES=myFork.cpp
export SOURCE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

all: make

make:
	@$(make) -e -f ${LIB_DIR}/lib_master.mk

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
 
