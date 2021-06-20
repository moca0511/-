import subprocess
import time
import sys

def ERR_DEF():
        print("\n\nError.\r\n\r\n")
    


def Ntrip():
    try:
        proc = subprocess.Popen(["sh", "while.sh", "1"])
        time.sleep(2)
        cmd = ["sh", "while.sh", "2"]
        res = subprocess.call(cmd)
    finally:
        ERR_DEF()
    # => Error.


def Tcp():
    try:
        proc = subprocess.Popen(["sh", "while.sh", "1"])
        time.sleep(2)
        cmd = ["sh", "while.sh", "2"]
        res = subprocess.call(cmd)
    finally:
        ERR_DEF()
    # => Error.


num = len(sys.argv)

if (num == 1) or (num == 2):
    if (num == 1)or((sys.argv[1] == "-n") and(num == 2)):
        print("通常スタート1")
        Ntrip()
    elif(sys.argv[1] == "-t") and(num == 2):
        print("通常スタート2")
        Ntrip()
    else:
        print("引数エラー2")
else:
    print("引数エラー")
