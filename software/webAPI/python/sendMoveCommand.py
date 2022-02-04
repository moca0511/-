# -*- coding: utf-8 -*-
import requests
import socket
import pickle
import sys
import json

args = sys.argv

if len(args)!=6:
	print("tcp_server.py ip port mode lat lng")
	sys.exit(1)

IP = args[1]
PORT = args[2]
BUFFER_SIZE = 1024

json_data = {
    "mode": int(args[3]),
    "lat": float(args[4]),
    "lng" : float(args[5])
}

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((IP, int(PORT)))
	msg = pickle.dumps(json_data)
	s.send(msg)
