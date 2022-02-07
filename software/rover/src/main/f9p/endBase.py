
import os
import sys

# 自作関数
nowDir = os.path.dirname(os.path.abspath(__file__))
slackControl_Path = nowDir
sys.path.append(slackControl_Path)
import str2str

str2str.getBaseUbx.endNgrok_base(str2str.slackTokenPath)
str2str.getBaseUbx.endStr2str_base(str2str.slackTokenPath)
