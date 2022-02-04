#!/bin/bash
SERVER_TCP=`python3 ~/slackBot/sub/slackRead_url.py tcp1_server`
SERVER_URL=`echo $SERVER_TCP | cut -d ":" -f 2 | cut -c 3-`
SERVER_PORT=`echo $SERVER_TCP | cut -d ":" -f 3`
SEND_JSON=/home/pi/tcp_python/send.json
RECV_JSON=/home/pi/tcp_python/recv.json
echo $SERVER_TCP
echo $SERVER_URL
echo $SERVER_PORT
python3 /home/pi/tcp_python/tcp_client.py $SERVER_URL $SERVER_PORT $SEND_JSON $RECV_JSON
