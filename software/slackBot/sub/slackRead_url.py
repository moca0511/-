# python3.6
import json
import requests
import sys


"""
ngrok_urlをslackから検索したときに得られる文字列の例

@xxx: ngrok_url
serverHttp    http://localhost:6080    -> http://e3d1-240b-253-c781-f700-53c0-844f-7a0f-92c5.ap.ngrok.io
serverHttps    http://localhost:6081    -> https://da24-240b-253-c781-f700-53c0-844f-7a0f-92c5.ap.ngrok.io
mySSH         localhost:6022           -> tcp://0.tcp.ap.ngrok.io:10128
tcp1          localhost:8000         -> tcp://0.tcp.ap.ngrok.io:14753
"""

args = sys.argv

# slackのチャンネルからメッセージを読み込み処理


# Jsonファイル読み込み
# 引数:設定ファイル
# 戻り値:ファイルの中身全て
def getJsonFile(fileName):
    with open(fileName, 'r') as f:
        return json.loads(f.read())


# slackからjsonデータを取得
def getMessageLog(_url, _headersk, _params):
    res = requests.get(_url, headers=_headersk, params=_params)
    json_data = res.json()
    j = json.loads(res.text)
    messages = j["messages"]
    return messages


# 引数textを含む最新のメッセージをslackのメッセージリスト(引数messages)から検索
# 戻り値1:引数textを含む最新のメッセージ、
# 戻り値2:最新のメッセージから何個目のメッセージか
def searchLog_text(text, messages):
    for i, message in enumerate(messages):
        if text in message["text"]:
            return message["text"], i


# 複数行のメッセージからpoint番目のngrokのURLを取り出す
def getURL(text, point):
    s_text = text.split('\n')
    return (s_text[point][s_text[point].rfind('<') +
                          1:s_text[point].rfind('>')])


# 複数行のメッセージからpoint番目のngrokの管理用タグを取り出す
def getTag(text, point):
    s_text = text.split('\n')
    ss_text = s_text[point].split()
    return ss_text[0]


api_keys = getJsonFile("/home/rakuhoku/slackBot/sub/config/token.json")  # apiKeysをファイルから読み込み

# slack読み込み用データを設定
token = api_keys['token_user']
url_history = "https://slack.com/api/conversations.history"
payload = {"channel": api_keys['channel'], "limit": "50"}
header = {"Authorization": "Bearer {}".format(token)}

# メッセージの読み込み
messages = getMessageLog(url_history, header, payload)          # メッセージの読み込み
text1, num_NgrokOn = searchLog_text("ngrok_url", messages)
#text, num_BotEnd = searchLog_text("end slackbot", messages)
#text, num_BotStart = searchLog_text("start slackbot", messages)
#text, num_NgrokOff = searchLog_text("ok_Off", messages)

# slackBotとngrokの起動状態の確認
getFlg = 1  # ngrokとslackBotが起動中華の確認用変数
# num_XXXXは小さいほど最新の通知に近い
#if num_BotStart < num_BotEnd:
    # slackBot起動中
 #   if (num_NgrokOn < num_BotEnd) & (num_NgrokOn < num_NgrokOff):
        #print("slackBot, ngrok = 起動中")
  #      getFlg = 1
    #else:
   #    print("slackBot = 起動中, ngrok = 停止中")
#else:
 #   print("slackBot = 停止中")

# ngrokのURLとトンネル識別用タグの抽出
if getFlg == 1:
    for i in range(1, 5):
        if args[1]==getTag(text1, i):
            print(getURL(text1, i))
