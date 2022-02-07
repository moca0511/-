# 外部プログラム実行用
import subprocess
import os

# 強制終了時の処理用
import sys
import signal

# 自作関数
nowDir = os.path.dirname(os.path.abspath(__file__))
slackControl_Path = nowDir + "/../../../gitSubModule/base/baseRover/src/sub/"
print(slackControl_Path)
sys.path.append(slackControl_Path)
import getBaseUbxFromSlack as getBaseUbx


def sig_handler(signum, frame) -> None:
    sys.exit(1)


# str2str起動用コマンドを作成
def makeStr2strCmd():
    str = getBaseUbx.getTcp1_base("./slackControl/config/token.json")
    # print(str[6:])

    inCmd = "str2str -in tcpcli://" + str[6:]
    outCmd = "-out tcpsvr://:2102"
    # out = "-out serial://ttyACM0:115200#2103"

    cmd = inCmd + " " + outCmd
    return cmd


#slackから基準局の情報を読み取ってstr2strを起動
def strMain():
    try:
        signal.signal(signal.SIGTERM, sig_handler)

        # str2str用コマンドを作成
        cmd = makeStr2strCmd()
        print(cmd)

        # ログファイルのパスを作成
        logPath = os.path.dirname(os.path.abspath(__file__)) + '/log.log'
        print(logPath)

        # サブプロセスをスタート
        with open(logPath, 'w') as output_file:
            proc = subprocess.Popen(
                cmd, shell=True, stdout=output_file, stderr=output_file, text=True)

        # サブプロセスの完了を待つ
        result = proc.communicate()

    except KeyboardInterrupt:
        print("\n強制終了 ctrl+c")
    finally:
        print("ens")
        signal.signal(signal.SIGTERM, signal.SIG_IGN)
        signal.signal(signal.SIGINT, signal.SIG_IGN)
        signal.signal(signal.SIGTERM, signal.SIG_DFL)
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        getBaseUbx.endNgrok_base("./slackControl/config/token.json")
        getBaseUbx.endStr2str_base("./slackControl/config/token.json")


strMain()
