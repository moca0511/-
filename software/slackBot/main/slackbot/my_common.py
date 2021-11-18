# coding: utf-8

import json
AAA = ""
BBB = 0


def getJsonFile(fileName):
    with open(fileName, 'r') as f:
        return json.loads(f.read())


api_keys = getJsonFile("../config/token.json")  # apiKeysをファイルから読み込み
