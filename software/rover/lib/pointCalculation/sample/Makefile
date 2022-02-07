CC=g++
make = make --no-print-directory
export LIB=libvincenty.a
export SOURCES=vincenty.cpp
export CFLAGS = 
export ONLY_HEADERS=

SAMPLE_OUT[1]=sample-1.out
SAMPLE_OUT[2]=sample-2.out

SAMPLE_SOURCE[1]=vincenty_distance.cpp
SAMPLE_SOURCE[2]=vincenty_destination.cpp

export SOURCE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))../
LIB_DIR=${ROOT_PATH}lib
LIBS=${LIB:lib%.a=-l%}

all: allSample

allSample:
	@$(make) -e sample-1
	@$(make) -e sample-2

${SAMPLE_OUT[1]}:${SAMPLE_SOURCE[1]}
	@$(eval A = g++1)
	$(CC) -o ${SAMPLE_OUT[1]} ${SAMPLE_SOURCE[1]} -I${LIB_DIR} -L${LIB_LIB_DIR} $(CFLAGS) ${LIBS}
	
${SAMPLE_OUT[2]}:${SAMPLE_SOURCE[2]}
	@$(eval A = g++2)
	$(CC) -o ${SAMPLE_OUT[2]} ${SAMPLE_SOURCE[2]} -I${LIB_DIR} -L${LIB_LIB_DIR} $(CFLAGS) ${LIBS}
	
sample-1: make ${SAMPLE_OUT[1]} makeCall
sample-2: make ${SAMPLE_OUT[2]} makeCall

run-1:
	./${SAMPLE_OUT[1]} 35.4681 133.0486 35.472222 133.050556

run-2:
	./${SAMPLE_OUT[2]} 35.4681 133.0486 21.21518366 490.58216516

COMMENT-1:
	@echo errになる
	POINT-1:   35.46810000°,  133.04860000°
	POINT-2:   35.47222200°,  133.05055600°
	-->
	LENGTH:      490.58216516
	AZIMUTH-1:       21.21518366
	AZIMUTH-2:      201.21631869

COMMENT-2:
	@echo errになる
	POINT-1:   35.46810000°,  133.04860000°
	AZIMUTH-1:       21.21518366°
	LENGTH:      490.58216516m
	-->
	POINT-2:   35.47222200°,  133.05055600°
	AZIMUTH-2:      201.21631869°

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
