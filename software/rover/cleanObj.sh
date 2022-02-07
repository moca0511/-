#!/usr/bin/bash

myClean() {
	echo "$1"
	find "$1" -name "*.o" -type f -print0 | xargs -0 rm -f
	find "$1" -name "*.a" -type f -print0 | xargs -0 rm -f
	find "$1" -name "*.out" -type f -print0 | xargs -0 rm -f
	find "$1" -name "*.inc" -type f -print0 | xargs -0 rm -f
}

cd "$(dirname $0)" || exit
nowDir=$('pwd')

d="$nowDir"/src
myClean "$d"

d="$nowDir"/log
myClean "$d"

d="$nowDir"/obj
myClean "$d"
