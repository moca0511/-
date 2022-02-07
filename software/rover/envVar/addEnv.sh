#!/bin/bash

# sh ./addEnv.shでは環境変数に追加されないので注意
# source ./addEnv.sh

sh ./checkEnv.sh

echo " "

PATH_FILE=./pro4_rover_path.path

if [ ! -e ${PATH_FILE} ]; then
	echo "pro4_rover_path.pathが見つかりません"
	echo "source ./addEnv.shはenvVarディレクトリで実行してください"
	exit 1
fi

# shellcheck disable=SC1090
.  ${PATH_FILE}
export PRO4_ROVER_DIR_PATH
echo PRO4_ROVER_DIR_PATHを更新した

echo " "

sh ./checkEnv.sh
