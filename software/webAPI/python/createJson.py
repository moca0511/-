import requests
import socket
import pickle
import sys
import json

args = sys.argv

if len(args)!=5:
	print("tcp_server.py mode lat lng jsonpath")
	sys.exit(1)

JSON_PATH=args[4]

json_data = {
    "mode": int(args[1]),
    "lat": float(args[2]),
    "lng" : float(args[3])
}

with open(JSON_PATH, 'w') as f:
    json.dump(json_data, f, ensure_ascii=False, indent=4)
