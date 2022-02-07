#!/bin/bash

cd "$(dirname "$0")" || exit

# configなどを含めて良く考えてパスを通す
SERVER_TCP=$(python3 slackRead.py server tcp1_server)

SERVER_URL=$(echo "$SERVER_TCP" | cut -d ":" -f 2 | cut -c 3-)
SERVER_PORT=$(echo "$SERVER_TCP" | cut -d ":" -f 3)

SEND_JSON=/tmp/send.json
RECV_JSON=/tmp/recv.json

# 中間ファイルを作成
echo '{
	"mode": 1,
	"lat": 0.0,
	"lng": 0.0
}' > "$SEND_JSON"
mkfifo "$RECV_JSON"
#echo " " > "$RECV_JSON"

echo "$SERVER_TCP"
echo "$SERVER_URL"
echo "$SERVER_PORT"

python3 ./tcp_client.py "$SERVER_URL" "$SERVER_PORT" $SEND_JSON $RECV_JSON
