#!/bin/bash
cd "$(dirname "$0")" || exit

# 引数によって読み込む設定ファイルととngrokサーバを変更
SETTING_YML=ngrok_setting.yml
SETTING_REGION= # ymlファイルよりコマンドライン引数が優先される

if [ "$#" = 1 ]; then
	if [ "$1" = "base" ]; then
		SETTING_YML=ngrok_setting_base.yml
		SETTING_REGION=ap
	elif [ "$1" = "server" ]; then
		SETTING_YML=ngrok_setting_server.yml
		SETTING_REGION=jp
	elif [ "$1" = "rover" ]; then
		SETTING_YML=ngrok_setting_rover.yml
		SETTING_REGION=us
	elif [ "$1" = "deff" ]; then
		SETTING_REGION=eu
	else :
		echo "ngrok実行失敗"
		exit 1
	fi
fi

# ngrok起動コマンド
#--authtokenは-configより先に記述すること
#--regionはどこでもよさそう
/usr/local/bin/ngrok start --config=../config/ngrok_token.yml -config=../config/"${SETTING_YML}" -all --region="${SETTING_REGION}"

# yml設定ファイルについて
# bind_tls:true		>>	https
# bind_tls:false	>>	http
# bind_tls:both		>>	https, http
