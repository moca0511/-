# coding: utf-8

# slackBot用
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

# botMode取得用
import os
import sys
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
import my_common

# 正規表現用
import re

# ホスト名用, ipアドレス取得用
import socket


# ------------------------------
# 各種操作用関数
# ------------------------------


# 「ngrok_check_url」投稿時に表示される一つのトンネル情報(一行分)
# "public_url"はngrokのURLなので必須
def getNameURL(num, json_data):
    return "{0: <10}\t{1: <22} -> {2}".format(
        json_data["tunnels"][num]["name"],
        json_data["tunnels"][num]["config"]["addr"],
        json_data["tunnels"][num]["public_url"])


# 起動中のngrokへ接続を行いjsonを取得
# 10sまでは接続テストを繰り返す
def getMyJson(_url, errPrint=0):
    conectTestTime = 10  # 1s単位
    while(1):
        tryCount = 0
        try:
            response = requests.get(_url)
            r = json.loads(response.text)
            try:
                if r["tunnels"][0]["name"] == None:
                    if(tryCount >= conectTestTime):
                        return ""
                    print("continue")
                    tryCount += 1
                    time.sleep(1)
                    continue
            except:
                if(tryCount >= conectTestTime):
                    return ""
                tryCount += 1
                print("continue")
                time.sleep(1)
                continue
            return r
        except requests.exceptions.RequestException as e:
            if errPrint == 1:
                print("エラー : ", e)
            return ""


# ngrokから受けっとったjson内のngrok_tagと一致する部分を調べ、
# ngrok_tagやngrokのURLに関する情報を含む文字列を返す
def getJsonStr(ngrok_tag, json_data_fromNgrok):
    for k in range(4):
        if ngrok_tag == json_data_fromNgrok["tunnels"][k]["name"]:
            return getNameURL(k, json_data_fromNgrok)


# messageのtextから現在のモードを確認する関数
# テキストの形式は'[コマンド][スペース][mode]'の必要がある
# [xxx][スペース][mode]であることを確認
def getModeFromMSG(message):
    math = re.search(r'\w+\s\w+\s*$', message.body['text'])
    r = ""
    if math == None:  # mathがnone
        r = "all"
    else:
        math = re.search(r'\w+$', message.body['text'])
        if(math.group() == "deff") | (math.group() == "server") | (math.group() == "rover") | (math.group() == "base"):
            r = math.group()
        else:
            r = "err-" + math.group()
    return r


# 現在のモードで操作する必要があるコマンドかを引数messageから判定
# 戻り値1=必要, 0=不必要
def ifMode(message):
    math = getModeFromMSG(message)
    if(my_common.botMode != math) & (math != "all"):
        print("現在のモードでは読み込めないコマンドです[{0}]".format(message.body['text']))
        return 0
    return 1


# slackに現在のモードを伝えるための文字列作成
# my_common.botModeTagはslack上のメッセージがどのbotから送信されたものかを確かめる目印でもあるので、必ずメッセージに付け加えること
# (メッセージのどこにあるかは見やすさ優先で良い)
# slackは`で囲まれた場所が赤色になる
def getModePrint():
    tag = my_common.botModeTag
    return '(' + tag + '`' + my_common.botMode + '`' + ')'


ngrokStatusURL = 'http://localhost:4040/api/tunnels'    # ngrok接続情報を受けとるためのURL


# プロセスの起動状態を確認
# processNameを含むプロセスがあるかを確認
# 一つも存在しなければerrMsgを返す
def getProcess(processName, errMsg):
    flg = 0
    myStr = ""
    for proc in psutil.process_iter():
        try:
            if processName in proc.exe():
                flg = 1
                myStr += "プロセスID:" + str(proc.pid) + "\n" +\
                    "実行モジュール：" + proc.exe() + "\n" +\
                    "コマンドライン:" + str(proc.cmdline()) + "\n" + \
                    "カレントディレクトリ:" + proc.cwd() + "\n\n"
        except psutil.AccessDenied:
            # print("このプロセスへのアクセス権がありません。")
            print("", end="")
    if flg == 0:
        myStr = errMsg
    return myStr


# ngrokの起動状態確認
def getNgrokProcess():
    return getProcess('ngrok', "no ngrok")


# str2strの起動確認
def getStr2StrProcess():
    return getProcess('str2str', None)


# ------------------------------
# ボット応答用関数
# ------------------------------

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
    message.reply('私にメンションと言ってどうするのだ' + getModePrint())  # メンション


@listen_to('リッスン')
def listen_func(message):
    message.send('誰かがリッスンと投稿したようだ' + getModePrint())  # ただの投稿
    message.reply('君だね？' + getModePrint())  # メンション


@listen_to('help')
@respond_to('help')
def returnHelpMsg(message):
    helpMsg = "下記のメッセージをbotのいるチャンネルに書き込むことで動作します。"
    helpMsg += getModePrint()
    helpMsg += "\n\
・ngrok_on				…ngrokが未起動ならngrokを起動して、「ngrok_check_url」と書き込んだ時の動作を行います。\n\
・ngrok_off			    …ngrokが起動済みなら、ngorkを終了させます。\n\
・ngrok_re              …ngrok_off,ngrok_onを順に行い、ngrokを再起動する\
・ngrok_check_process	…起動済みのngrokのプロセスIDなどを表示します\n\
・ngrok_check_url		…起動済みngrokの内4040ポートからデータを得たデータを表示する。\n\
・ngrok_check_all		…[ngrok_check_process][ngrok_check_url]と順に書き込んだ時の動作を行う。\n\
・getMode		    	…起動中の全てのbotが現在のモードを表示\n\
・getIp			    	…botを起動しているサーバのホスト名とIPアドレスを表示\n\
・ping_aa.bb.cc.dd  	…botを起動してるサーバのIPアドレスとaa.bb.cc.ddを比較(aa,bb,cc,ddは数字出なければならない)\n\
・checkMode XXX			…botの現在モードとXXXを比較(XXXが空白の場合all扱い)\n\
・reboot                …slackbotを終了しコンピュータを再起動します(終了するモードを直接指定する必要がある)\
・help              	…slackに入力すると反応する文字列や動作を全てのボットが表示\
\n\
- 送信先モードについて\n\
入力は下記の2パターンを想定\n\
1.コマンド[スペース][モード]\n\
2.コマンド\n\
\n\
入力例1の時は[モード]のbotだけが反応\n\
入力例2の時は全てのbotが反応\n\
getMode,helpだけは全てのbotが反応する\
\n\
\n\
- baseモードのみの実装機能\n\
・f9p_on			…str2strを起動後、ngrokを起動します\n\
・f9p_off		   …str2strを終了します\
"
    message.reply(helpMsg)


# python3のプログラム(slackbot)を終了しコンピュータを再起動します
@listen_to('reboot')
@respond_to('reboot')
def myReboot_func(message):
    if(my_common.botMode == getModeFromMSG(message)):
        host = socket.gethostname() + ".local"
        message.reply(host + "のrebootを行います" + getModePrint())
        subprocess.run(('sudo', '/sbin/shutdown', '-h', '1', '-r'))  # 1分後に再起動
        subprocess.run(('pkill', 'python3'))
    else:
        print("現在のモードでは読み込めないコマンドです[{0}]".format(message.body['text']))


# ngrokの再起動
@listen_to('ngrok_re')
@respond_to('ngrok_re')
def ngrok_re_func(message):
    ngrok_off_func(message)
    ngrok_on_func(message)


@listen_to('ngrok_on')
@respond_to('ngrok_on')
def ngrok_on_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    json_data = getMyJson(ngrokStatusURL)  # ngrok起動状態確認
    if json_data == "":
        # ngrok起動プログラムに現在のモードを引数として渡し、実行
        proc = subprocess.Popen(["bash", "./../ngrok/ngrok_start.sh", my_common.botMode],
                                stdout=PIPE,
                                stderr=PIPE)
        #proc2 = subprocess.run(['ls', '-la', './../ngrok'])
        message.reply('ok_On' + getModePrint())
        time.sleep(2)  # ngrok起動の待機時間
        ngrok_check_url_func(message)
    else:
        message.reply("既にngrokは起動しています" + getModePrint())


@respond_to('ngrok_off')
@listen_to('ngrok_off')
def ngrok_off_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    json_data = getMyJson(ngrokStatusURL)  # ngrok起動状態確認
    if json_data == "":
        message.reply("ngrokが既に終了ています" + getModePrint())
        return
    proc2 = subprocess.run(['pkill', 'ngrok'], stdout=PIPE, stderr=PIPE)
    #proc2 = subprocess.run(['ls', '-la', './../ngrok'])
    message.reply('ok_Off' + getModePrint())


@listen_to('ngrok_check_process')
@respond_to('ngrok_check_process')
def ngrok_check_process_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    myStr = getNgrokProcess()   # ngrokのプロセス状態などの情報を取得
    message.reply("ngrok_process" +
                  getModePrint() + "\n" + myStr)


@listen_to('ngrok_check_url')
@respond_to('ngrok_check_url')
def ngrok_check_url_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    json_data = getMyJson(ngrokStatusURL)  # ngrok起動状態確認
    if json_data == "":
        message.reply("ngrokが起動していません" + getModePrint())
        return

    # ngrokの情報必要なものだけ抜き出し、既定の順に並び替え
    myStr = ""
    for j in my_common.ngrokTag:
        myStr += getJsonStr(j, json_data) + "\n"
    message.reply("ngrok_url" + getModePrint() + "\n" + myStr)


@listen_to('ngrok_check_all')
@respond_to('ngrok_check_all')
def ngrok_check_all_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    ngrok_check_process_func(message)
    ngrok_check_url_func(message)


@listen_to('f9p_on')
@respond_to('f9p_on')
def f9p_on_func(message):
    if(my_common.botMode != "base"):
        print("現在のモードでは読み込めないコマンドです[{0}]".format(message.body['text']))
        return
    math = getModeFromMSG(message)
    if(my_common.botMode != math):
        message.reply("送信先モードがおかしいです" + getModePrint())
        return
    else:
        strMsg = "f9p_on_ok" + getModePrint() + "\n"
        if(getStr2StrProcess() == None):
            proc = subprocess.Popen(["bash", "./../f9p_base/ubxTcpSrv.sh"],
                                    stdout=PIPE,
                                    stderr=PIPE)
            time.sleep(2)  # str2str起動の待機時間
            subMsg = getStr2StrProcess()
            if(subMsg != None):
                strMsg += subMsg
            else:
                strMsg += "str2str起動に失敗しました"
            message.reply(strMsg)
            ngrok_on_func(message)
        else:
            strMsg += "str2strは起動済みです"
            message.reply(strMsg)


@listen_to('f9p_off')
@respond_to('f9p_off')
def f9p_off_func(message):
    if(my_common.botMode != "base"):
        print("現在のモードでは読み込めないコマンドです[{0}]".format(message.body['text']))
        return
    math = getModeFromMSG(message)
    if(my_common.botMode != math):
        message.reply("送信先モードがおかしいです" + getModePrint())
        return
    else:
        strMsg = "f9p_off_ok" + getModePrint() + "\n"
        if(getStr2StrProcess() == None):
            strMsg += "str2strは起動していません"
        else:
            strMsg += "str2strは終了しました"
            proc2 = subprocess.run(
                ['pkill', 'str2str'], stdout=PIPE, stderr=PIPE)
        message.reply(strMsg)


@listen_to('getMode')
@respond_to('getMode')
def getMode_func(message):
    msg = "現在のモードは"
    if(my_common.botMode == ""):
        msg += "(NULL)"
    msg += my_common.botMode
    message.reply(msg)


# 基本の返事
@default_reply()
def default_func(message):
    message.reply('高性能なデフォルトの返事です' + getModePrint())


# ホスト名とipアドレスを表示
@listen_to('getIp')
@respond_to('getIp')
def getIp_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    host = socket.gethostname() + ".local"
    ip = socket.gethostbyname(host)
    message.reply("{0}=ip({1})".format(host, ip) +
                  getModePrint())


# ping ipアドレスでbotサーバのipアドレスと入力文字を比較する
# 正規表現による受信テスト
# 指定する正規表現の中に^や$を入れないと、文字列の何処かにこの正規表現に引っかかる場合にも反応してしまう。
@listen_to('ping_')
@respond_to('ping_')
def getPing_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    host = socket.gethostname() + ".local"
    ip = socket.gethostbyname(host)
    math = re.search(r'\d+.\d+.\d+.\d+', message.body['text'])
    str = math.group()
    str += 'はpingのコマンドですね。\n'
    if ip == math.group():
        str += "私のアドレスですね"
    else:
        str += "私のアドレスは{0}ですが".format(ip)
    message.reply(str + '\n' + getModePrint())


# 現在モードとcheckMode XXXを比較(XXXが空白の場合all扱い)
@listen_to('checkMode')
@respond_to('checkMode')
def checkMode_func(message):
    if(ifMode(message) == 0):  # メッセ時から反応するべきモード化判定
        return
    math = getModeFromMSG(message)
    str = "現在モードは{0}です\n読み込んだモード={1}\n".format(
        my_common.botMode, math)
    if(my_common.botMode == math):
        str += "検索文字と現在のモードをが一致"
    elif(math == "all"):
        str += "検索文字がないのでall読み込み"
    else:
        str += "検索文字と現在のモードをが不一致"
    message.reply(str + '\n' + getModePrint())
