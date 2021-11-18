# coding: utf-8

from slackbot.bot import Bot
import sys

import requests
import my_common


def sendMessage(url, header, ch, text):
    data = {'channel': ch, 'text': text}
    return requests.post(url, headers=header, data=data)


token_user = my_common.api_keys['token_user']
url_send = "https://slack.com/api/chat.postMessage"
head = {"Authorization": "Bearer {}".format(token_user)}


def main():
    bot = Bot()
    bot.run()


if __name__ == "__main__":
    print('start slackbot')
    if(sys.argv[1] != "secret"):
        sendMessage(url_send, head,
                    my_common.api_keys['channel'], 'start slackbot')
    try:
        main()
    except:
        if(sys.argv[1] != "secret"):
            sendMessage(url_send, head,
                        my_common.api_keys['channel'], 'end slackbot')
        print("")
