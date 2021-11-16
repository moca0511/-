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

data = json.dumps(json_data)
print(data)


response = requests.post(
    url,
    data
    )

res_data = response.json()

print(res_data)
