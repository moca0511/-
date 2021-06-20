#!/bin/bash

#---------------------------
# 概要
#---------------------------
# リアルタイムRTK処理をＣＵＩで行うプログラム
# bashで動作する。
# また、rtkrcvを実行するディレクトリにrtkrcv.navが必要だが、原因が不明

#---------------------------
#引数の数について
#---------------------------

#引数は２つまで設定可能

#引数1が-i1sの時、シリアル通信から移動局のデータを受信
#引数1が-i1tの時、TCP通信から移動局のデータを受信

# 引数2は引数1を指定している場合だけしか、指定できない。
# 引数2が-i2tの時、TCP通信から基準局のデータ受信
# 引数2が-i2nの時、rtk2go.comなどのNtripsから基準局のデータ受信


#---------------------------
#引数の数を確認
#---------------------------
if [ $# != 2 ] && [ $# != 1 ] && [ $# != 0 ] ; then
    echo 引数エラー: $*
    exit 1
else
    echo OK
fi
#---------------------------


#---------------------------
# 設定ファイルの読み込み
#---------------------------
. ./config_master.txt
#---------------------------



#---------------------------
#　読み込んだデータを現在の環境に修正
#---------------------------

#シェルの引数でもrtkrcvのコンフィグファイルを選択できるようにするべき?
#ログファイルの設定について確認するべき
#利用したコンフィグファイルをログディレクトリに挿入するべき

appDir=${masterDirPath}/RTKLIB/app/rtkrcv/gcc/


# 現在時刻取得
nowTime=`date "+%Y-%m-%d_%Hh%Mm%Ss"`



echo ${nowTime}
config_items[0]="logstr1-path       ="
config_items[1]="logstr2-path       ="

config_items[2]="outstr1-path       ="
config_items[3]="outstr2-path       ="

config_items[4]="ant2-pos1          ="
config_items[5]="ant2-pos2          ="
config_items[6]="ant2-pos3          ="


config_items[7]="inpstr1-type       ="
config_items[8]="inpstr2-type       ="

config_items[9]="inpstr1-path       ="
config_items[10]="inpstr2-path       ="

config_data[0]="${logDirPath}/${nowTime}/rover.txt"
config_data[1]="${logDirPath}/${nowTime}/base.txt"
config_data[2]=":@:${tcpPortResult}/::"
config_data[3]="${logDirPath}/${nowTime}/result.pos::T::S=1"
config_data[4]="${mountLat} # (deg|m)"
config_data[5]="${mountLon} # (deg|m)"
config_data[6]="${mountHei} # (deg|m)"


inpstr1type=1 # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
if ( [ $# == 1 ] || [ $# == 2 ]) && [ $1 = "-i1s" ] || [ $# == 0 ]; then
	inpstr1type=1 # シリアル
	echo "input1 mode シリアル"
	config_data[7]="serial     # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
	config_data[9]="ttyACM0:${usbSpeed}:7:n:1:off"

elif ( [ $# == 1 ] || [ $# == 2 ] ) && [ $1 = "-i1t" ] ; then
	inpstr1type=4 # tcpcli
	echo "input1 mode tcpcli"
	config_data[7]="tcpcli     # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
	config_data[9]="127.0.0.1:${tcpPortRawRover}"
else
    echo 引数エラー: $*
	exit 1
fi

inpstr2type=7 # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
if [ $# == 2 ] && [ $2 = "-i2t" ]; then
	inpstr2type=4 # tcpcli
	echo "input2 mode tcp"
	config_data[8]="tcpcli     # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
	config_data[10]="${tcpHostnameRawBase}:${tcpPortRawBase}"
elif [ $# == 2 ]  && [ $2 = "-i2n" ] || [ $# == 0 ]  || [ $# == 1 ] ; then
	inpstr2type=7 # ntripcli
	echo "input2 mode ntripcli"
	config_data[8]="ntripcli     # (0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http)"
	config_data[10]=":@rtk2go.com:2101/${mountPoint}"
else
    echo 引数エラー: $*
	exit 1
fi

rtkConfigFile=${masterDirPath}/log/${nowTime}/rtkConfigFile.conf

#---------------------------
# 読み込んだデータの確認
#---------------------------

echo 
echo ${config_data[7]}
echo ${config_data[9]}
echo 
echo ${config_data[8]}
echo ${config_data[10]}
echo 

#---------------------------
# 今回使用する設定ファイル並びにログディレクトリの作成
#---------------------------

#echo ${#config_items[@]}

# mkdir ${masterDirPath}/log/
mkdir ${masterDirPath}/log/${nowTime}
echo -n > ${rtkConfigFile}
echo ${rtkConfigFile}

sed -ne '/# RTKNAVI/,$p' ./config_master.txt > ${rtkConfigFile}



for((i=0; i<${#config_items[@]}; i++))
do
	echo ${i}
    sed -i -e "s!${config_items[i]}!${config_items[i]}${config_data[i]} #!g" ${rtkConfigFile}  
done
#---------------------------


#---------------------------
# リアルたいむ処理の開始
#---------------------------

#sed -i -e "s@${config_items[0]}@${config_items[0]}${config_data[0]}@g" ./outConfig.txt
#cd ${appDir}
#sudo ./rtkrcv -s -o ${CONFIG_FILE} -s -d /dev/tty0 -t 5 -r 1
#sudo ./rtkrcv -s -o ${CONFIG_FILE} -s -d /dev/tty0

sudo ${appDir}/rtkrcv -s -o ${rtkConfigFile}  -d /dev/tty0
