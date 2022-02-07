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


def sendMessageList(list, tokenPath="config/token.json", printFlg=False):
    api_keys = getJsonFile(tokenPath)  # apiKeysをファイルから読み込み

    # slack送信用データを設定
    token = api_keys['token_user']
    url_send = "https://slack.com/api/chat.postMessage"
    head = {"Authorization": "Bearer {}".format(token)}
    channelId = api_keys['channel']

    # 送信用テキストデータの設定
    #mySendMessage = ['f9p_on base', 'ngrok_on base', 'ngrok_check_url base']
    mySendMessage = list
    for i in mySendMessage:
        r = sendMessage(url_send, head, channelId, i)
        if (printFlg == True):
            print("return ", r.json())

if __name__ == "__main__":
    # 送信用テキストデータの設定
    args = sys.argv
    mySendMessage = 'メッセージ送信のテストです。'
    if 2 <= len(args):
        mySendMessage = args[1]
    list = [mySendMessage]
    r = sendMessageList(list, printFlg=True)
