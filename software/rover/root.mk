export ROOT_PATH = $(realpath $(dir $(lastword $(MAKEFILE_LIST))))/
export make = make --no-print-directory
# Makefile_header
export LIB_DIR=${ROOT_PATH}lib
export LIB_LIB_DIR=${ROOT_PATH}obj/lib

export INCLUDE_DIR=${LIB_DIR}

#make = make --no-print-directory
#RM = rm -f
#CC=g++
#all:
#	echo ${ROOT_PATH}
