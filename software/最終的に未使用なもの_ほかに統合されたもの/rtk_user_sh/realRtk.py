import subprocess
import time
import sys


# プログラム終了時に実行 (Ctrl + C など)
def ERR_DEF():
        print("\n\nError.\r\n\r\n")
        cmd = ["./str2str.out", "-k"]
        res = subprocess.call(cmd)


# baseデータをRtk2go.comなどのNtripsから受信したいとき
def Ntrip():
    try:
        proc = subprocess.Popen(["./str2str.out",  "-rn"])
        time.sleep(2)
        cmd = ["bash", "rtkrcv.sh", "-i1t", "-i2n"]
        res = subprocess.call(cmd)
    finally:
        ERR_DEF()
    # => Error.

# baseデータをローカルネットワーク内から受信したいとき
def Tcp():
    try:
        proc = subprocess.Popen(["./str2str.out",  "-rn"])
        time.sleep(2)

        cmd = ["bash", "rtkrcv.sh", "-i1t", "-i2t"]
        res = subprocess.call(cmd)
    finally:
        ERR_DEF()
    # => Error.


num = len(sys.argv)

if (num == 1) or (num == 2):
    if (num == 1)or((sys.argv[1] == "-n") and(num == 2)):
        print("通常スタート1")  #引数がない場合か、引数が-n(ntrips)のみの時
        Ntrip()
    elif(sys.argv[1] == "-t") and(num == 2):
        print("通常スタート2")  #引数が-t(tcp)の時
        Tcp()
    else:
        print("引数エラー2")
else:
    print("引数エラー")
