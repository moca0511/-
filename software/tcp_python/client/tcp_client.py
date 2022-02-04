# -*- coding: utf-8 -*-

import requests
import socket
import pickle
import json
import time
import sys
import os
import subprocess

from time import  sleep

# 
# $python3 /path/tcp_client.py ip port /path/json.fifo
# fifoからjson文字列を読み取り、websocketにて指定IP、PORTに送信
#　tcp_server.pyにて、先にip,portが起動している必要あり
#

def main():
    args = sys.argv

    if len(args)!=5:
        print("tcp_client.py ip port path/send.json path/recv.json")
        sys.exit(1)

    IP = args[1]
    PORT = args[2]
    BUFFER_SIZE = 1024
    JSON_SEND_PATH=args[3]
    JSON_RCV_PATH=args[4]
    #try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((IP, int(PORT)))
        while True:
            try:
                s.settimeout(1.0)
                msg=s.recv(BUFFER_SIZE)
                d = pickle.loads(msg)
                print(json.dumps(d, indent=4))
                with open(JSON_RCV_PATH, 'w') as wf:
                    json.dump(d, wf, ensure_ascii=False, indent=4)
                s.send(msg)
            except socket.timeout:
                with open(JSON_SEND_PATH, 'r') as rf:
                    
                    json_data=json.load(rf)
                    json_str = json.dumps(json_data)
                    print("send : " + json_str)
                    msg = pickle.dumps(json_data)
                    s.send(msg)
                continue
            except:
                print("error\n")
                exit(1)
            

if __name__ == "__main__":
    main()
