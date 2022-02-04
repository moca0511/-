import requests
import json
import sys

url = "https://httpbin.org/post"
args = sys.argv


json_data = {
    "mode": args[1],
    "lat": args[2],
    "lng" : args[3]
}

JSON_PATH=args[4]

data = json.dumps(json_data)
print(data)
with open(JSON_PATH, 'w') as f:
    json.dump(json_data, f, ensure_ascii=False, indent=4)
