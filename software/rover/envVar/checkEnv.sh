#!/bin/bash

if [ "${PRO4_ROVER_DIR_PATH:+foo}" ]; then
	echo PRO4_ROVER_DIR_PATHがある
	printenv PRO4_ROVER_DIR_PATH
else
	echo PRO4_ROVER_DIR_PATHがない
fi
