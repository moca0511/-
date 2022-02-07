# python3.6
import json
import requests
import sys


# Jsonファイル読み込み
# 引数:設定ファイル
# 戻り値:ファイルの中身全て
def getJsonFile(fileName):
    with open(fileName, 'r') as f:
        return json.loads(f.read())


# slackのチャンネルにメッセージ送信
# 戻り値:送信結果のjson
def sendMessage(url, header, chId, text):
    data = {'channel': chId, 'text': text}
    return requests.post(url, headers=header, data=data)


if __name__ == "__main__":

    api_keys = getJsonFile("./config/token.json")  # apiKeysをファイルから読み込み

    # slack送信用データを設定
    token = api_keys['token_user']
    url_send = "https://slack.com/api/chat.postMessage"
    head = {"Authorization": "Bearer {}".format(token)}
    channelId = api_keys['channel']

    # 送信用テキストデータの設定
    args = sys.argv
    mySendMessage = 'メッセージ送信のテストです。'
    if 2 <= len(args):
        mySendMessage = args[1]

    # メッセージの送信と結果の読み込み
    r = sendMessage(url_send, head, channelId, mySendMessage)
    print("return ", r.json())  # 送信成功と失敗の確認用
