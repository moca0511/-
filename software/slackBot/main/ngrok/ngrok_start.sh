#!/bin/bash
cd "$(dirname "$0")" || exit

#--authtokenは-configより先に記述すること
#--regionはどこでもよさそう
ngrok start --config=../config/ngrok_token.yml  -config=../config/ngrok_setting.yml  -all

# 設定ファイルについて
# 
# bind_tls:true		>>	https
# bind_tls:false	>>	http
# bind_tls:both		>>	https, http
