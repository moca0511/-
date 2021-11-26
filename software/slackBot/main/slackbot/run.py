# coding: utf-8

from slackbot.bot import Bot
import sys
import requests
# 自作関数
import my_common

import signal


# url,header,chから得られるワークスペースのチャンネルにtextを送信
def sendMessage(url, header, ch, text):
    data = {'channel': ch, 'text': text}
    return requests.post(url, headers=header, data=data)


def sig_handler(signum, frame) -> None:
    sys.exit(1)

def main():
    bot = Bot()
    bot.run()


if __name__ == "__main__":

    # キー情報の取得
    token_user = my_common.api_keys['token_user']
    url_send = "https://slack.com/api/chat.postMessage"
    head = {"Authorization": "Bearer {}".format(token_user)}

    if(len(sys.argv) != 2):
        # 引数の数がおかしいのでデフォルトモード
        my_common.botMode = "deff"
        myCh = my_common.api_keys['channel']
        ngrokSettingFile = "./../config/ngrok_setting.yml"
    else:
        # -引数によるモードチェック-
        # モードによって対応チャンネルを変更する計画が存在するのでmyChを利用。
        # 計画が破棄されたときは適当な方法を使う(forもあり？)
        my_common.botMode = sys.argv[1]
        if(sys.argv[1] == "server"):
            ngrokSettingFile = "./../config/ngrok_setting_server.yml"
            myCh = my_common.api_keys['channel']
        elif(sys.argv[1] == "base"):
            ngrokSettingFile = "./../config/ngrok_setting_base.yml"
            myCh = my_common.api_keys['channel']
        elif(sys.argv[1] == "rover"):
            ngrokSettingFile = "./../config/ngrok_setting_rover.yml"
            myCh = my_common.api_keys['channel']
        else:
            # 基本的にdeffモードで起動 main_start.shが常に引数を渡すため
            ngrokSettingFile = "./../config/ngrok_setting.yml"
            my_common.botMode = "deff"
            myCh = my_common.api_keys['channel']
    startMsg = 'start slackbot : ' + my_common.botModeTag + \
        '`' + my_common.botMode + '`' + '\n\n'

    my_common.ngrokTag = my_common.getNgrokKeyFromYml(ngrokSettingFile)

    sendMessage(url_send, head, myCh, startMsg)

    print(startMsg)
    signal.signal(signal.SIGTERM, sig_handler)
    try:
        main()
    except:
        signal.signal(signal.SIGTERM, signal.SIG_DFL)

        str = 'end slackbot: ' + my_common.botModeTag + \
            '`' + my_common.botMode + '`' + '\n\n'
        sendMessage(url_send, head, myCh, str)
        print("")
