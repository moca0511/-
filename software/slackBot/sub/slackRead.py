# python3.6
import json
import requests
import sys

# slackのチャンネルからメッセージを読み込み処理

"""
ngrok_urlをslackから検索したときに得られる文字列の例

@xxx: ngrok_url
serverHttp    http://localhost:6080    -> http://e3d1-240b-253-c781-f700-53c0-844f-7a0f-92c5.ap.ngrok.io
serverHttps    http://localhost:6081    -> https://da24-240b-253-c781-f700-53c0-844f-7a0f-92c5.ap.ngrok.io
mySSH         localhost:6022           -> tcp://0.tcp.ap.ngrok.io:10128
tcp1          localhost:8000         -> tcp://0.tcp.ap.ngrok.io:14753
"""


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
# 戻り値2:最新のメッセージから何個目のメッセージか(999は読み込むメッセージの最大数より多い必要がある)
def searchLog_text(text, messages, mode):
    tag = "botMode = " + '`' + mode + '`'
    for i, message in enumerate(messages):
        if (tag in message["text"]):
            if (text in message["text"]):
                return message["text"], i
    return None, 999


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


def getSlackMsg(mode):
    api_keys = getJsonFile("./config/token.json")  # apiKeysをファイルから読み込み

    # slack読み込み用データを設定
    token = api_keys['token_user']
    url_history = "https://slack.com/api/conversations.history"
    payload = {"channel": api_keys['channel']}  # "limit": "50"
    header = {"Authorization": "Bearer {}".format(token)}

    # メッセージの読み込み
    messages = getMessageLog(
        url_history, header, payload)          # メッセージの読み込み
    text, num_BotEnd = searchLog_text("end slackbot", messages, mode)
    text, num_BotStart = searchLog_text("start slackbot", messages, mode)
    text, num_NgrokOff = searchLog_text("ok_Off", messages, mode)
    text1, num_NgrokOn = searchLog_text("ngrok_url", messages, mode)

    text, num_f9pOn = searchLog_text("f9p_on_ok", messages, mode)
    text, num_f9pOff = searchLog_text("f9p_off_ok", messages, mode)

    """
    print(num_BotEnd)
    print(num_BotStart)
    print(num_NgrokOff)
    print(num_NgrokOn)
    """

    # slackBotとngrokの起動状態の確認
    getFlg = 0  # ngrokとslackBotが起動中華の確認用変数
    # num_XXXXは小さいほど最新の通知に近い
    rData = 0
    if num_BotStart < num_BotEnd:
        # slackBot起動中
        if (num_NgrokOn < num_BotEnd) & (num_NgrokOn < num_NgrokOff):
            if(mode == "base") & (num_f9pOn < num_f9pOff):
                rData = 4
            else:
                rData = 3
        else:
            rData = 2
    else:
        rData = 1
    return rData, text1


if __name__ == "__main__":
    if(len(sys.argv) != 2):
        mode = "deff"
    else:
        if (sys.argv[1] == "server") | (sys.argv[1] == "base") | (sys.argv[1] == "rover"):
            mode = sys.argv[1]
        else:
            # 基本的にdeffモードで起動 main_start.shが常に引数を渡すため
            mode = "deff"

    print("nowMode=" + mode)

    rData, text1 = getSlackMsg(mode)

    if rData == 4:
        print("slackBot, ngrok, str2str = 起動中")
    elif rData == 3:
        print("slackBot, ngrok = 起動中")
    elif rData == 2:
        print("slackBot = 起動中, ngrok = 停止中")
    elif rData == 1:
        print("slackBot = 停止中")

    # ngrokのURLとトンネル識別用タグの抽出
    if rData >= 3:
        print("抽出")
        for i in range(1, 5):
            print(getTag(text1, i) + " : ", end="")
            print(getURL(text1, i))
