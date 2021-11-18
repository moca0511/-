# coding: utf-8

from slackbot.bot import respond_to  # @botname: で反応するデコーダ
from slackbot.bot import listen_to  # チャネル内発言で反応するデコーダ
from slackbot.bot import default_reply  # 該当する応答がない場合に反応するデコーダ

# shell用
import subprocess
from subprocess import PIPE

# プロセスチェック
import psutil

# ngrok チェック
import json
import requests

# sleep用
import time


def getNameURL(num, json_data):
    return "{0: <10}\t{1: <22} -> {2}".format(
        json_data["tunnels"][num]["name"],
        json_data["tunnels"][num]["config"]["addr"],
        json_data["tunnels"][num]["public_url"])


def getMyJson(_url, errPrint=0):
    try:
        response = requests.get(_url)
        return json.loads(response.text)
    except requests.exceptions.RequestException as e:
        if errPrint == 1:
            print("エラー : ", e)
        return ""


def getJsonStr(j, json_data):
    for k in range(4):
        if j == json_data["tunnels"][k]["name"]:
            return getNameURL(k, json_data)


ARRAY_HTTP = ["serverHttp", "serverHttps"]
ARRAY_TCP = ["mySSH", "tcp1"]
ngrokStatusURL = 'http://localhost:4040/api/tunnels'


def getNgrokProcess():
    flg = 0
    myStr = ""
    for proc in psutil.process_iter():
        try:
            if 'ngrok' in proc.exe():
                flg = 1
                myStr += "プロセスID:" + str(proc.pid) + "\n" +\
                    "実行モジュール：" + proc.exe() + "\n" +\
                    "コマンドライン:" + str(proc.cmdline()) + "\n" + \
                    "カレントディレクトリ:" + proc.cwd() + "\n\n"
        except psutil.AccessDenied:
            # print("このプロセスへのアクセス権がありません。")
            print("", end="")
    if flg == 0:
        myStr = "no ngrok"
    return myStr


# @respond_to('string')     bot宛のメッセージ
#                           stringは正規表現が可能 「r'string'」
# @listen_to('string')      チャンネル内のbot宛以外の投稿
#                           @botname: では反応しないことに注意
#                           他の人へのメンションでは反応する
#                           正規表現可能
# @default_reply()          DEFAULT_REPLY と同じ働き
#                           正規表現を指定すると、他のデコーダにヒットせず、
#                           正規表現にマッチするときに反応
#                           ・・・なのだが、正規表現を指定するとエラーになる？


# message.reply('string')   @発言者名: string でメッセージを送信
# message.send('string')    string を送信
# message.react('icon_emoji')  発言者のメッセージにリアクション(スタンプ)する
#                               文字列中に':'はいらない
@respond_to('メンション')
def mention_func(message):
    message.reply('私にメンションと言ってどうするのだ')  # メンション


@listen_to('リッスン')
def listen_func(message):
    message.send('誰かがリッスンと投稿したようだ')  # ただの投稿
    message.reply('君だね？')  # メンション


@listen_to('help')
@respond_to('help')
def returnHelpMsg(message):
    helpMsg = "下記のメッセージをbotのいるチャンネルに書き込むことで動作します。\n\
・ngrok_on		…ngrokが未起動ならngrokを起動して、「ngrok_check_url」と書き込んだ時の動作を行います。\n\
・ngrok_off		…ngrokが起動済みなら、ngorkを終了させます。\n\
・ngrok_check_process	…起動済みのngrokのプロセスIDなどを表示します\n\
・ngrok_check_url	…起動済みngrokの内4040ポートからデータを得たデータを表示する。\n\
・ngrok_check_all	…[ngrok_check_process][ngrok_check_url]と順に書き込んだ時の動作を行う。"
    message.reply(helpMsg)


@listen_to('ngrok_on')
@respond_to('ngrok_on')
def ngrok_on_func(message):
    json_data = getMyJson(ngrokStatusURL)
    if json_data == "":
        proc = subprocess.Popen(["bash", "./../ngrok/ngrok_start.sh"],
                                stdout=PIPE,
                                stderr=PIPE)
        #proc2 = subprocess.run(['ls', '-la', './../ngrok'])
        message.reply('ok_On')
        time.sleep(1)
        ngrok_check_url_func(message)
    else:
        message.reply("既にngrokは起動しています")


@respond_to('ngrok_off')
@listen_to('ngrok_off')
def ngrok_off_func(message):
    json_data = getMyJson(ngrokStatusURL)
    if json_data == "":
        message.reply("ngrokが既に終了ています")
        return
    proc2 = subprocess.run(['pkill', 'ngrok'], stdout=PIPE, stderr=PIPE)
    #proc2 = subprocess.run(['ls', '-la', './../ngrok'])
    message.reply('ok_Off')


@listen_to('ngrok_check_process')
@respond_to('ngrok_check_process')
def ngrok_check_process_func(message):
    myStr = getNgrokProcess()
    message.reply("ngrok_process\n" + myStr)


@listen_to('ngrok_check_url')
@respond_to('ngrok_check_url')
def ngrok_check_url_func(message):
    json_data = getMyJson(ngrokStatusURL)
    if json_data == "":
        message.reply("ngrokが起動していません")
        return
    myStr = ""
    for j in ARRAY_HTTP:
        myStr += getJsonStr(j, json_data) + "\n"

    for j in ARRAY_TCP:
        myStr += getJsonStr(j, json_data) + "\n"
    message.reply("ngrok_url\n" + myStr)


@listen_to('ngrok_check_all')
@respond_to('ngrok_check_all')
def ngrok_check_all_func(message):
    ngrok_check_process_func(message)
    ngrok_check_url_func(message)


@listen_to('f9p_on')
@respond_to('f9p_on')
def f9p_on(message):
    message.reply("未設定です\n")


@listen_to('f9p_off')
@respond_to('f9p_off')
def f9p_off(message):
    message.reply("未設定です\n")
