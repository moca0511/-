# coding: utf-8
import yaml
import json
AAA = ""
BBB = 0


def getJsonFile(fileName):
    with open(fileName, 'r') as f:
        return json.loads(f.read())


# ymlファイルを読み込んでjsonとして読み込む
def yml2json(yamlFileName):
    with open(yamlFileName, 'rb') as file:
        # YAMLから辞書
        obj = yaml.safe_load(file)
        # 辞書からJSON
        j_str = json.dumps(obj, indent=2)
        j = json.loads(j_str)
        return j


# ngrokの設定ファイルから通信用タグをファイルに記載されている順で取得
def getNgrokKeyFromYml(yamlFileName):
    js = yml2json(yamlFileName)
    l = []  # 空のリスト用意
    for jsn_key in js['tunnels']:
        l.append(jsn_key)
    return l


ngrokTag = []

# botModeTagはslack上のメッセージがどのbotから送信されたものかを確かめる目印でもあるので、変更には注意
botModeTag = "botMode = "

api_keys = getJsonFile("../config/token.json")  # apiKeysをファイルから読み込み

botMode = ""
