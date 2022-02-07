
import os
import sys

# 自作関数
nowDir = os.path.dirname(os.path.abspath(__file__))
slackControl_Path = nowDir 
sys.path.append(slackControl_Path)
import str2str


if __name__ == '__main__':
    print(str2str.makeStr2strCmd())
