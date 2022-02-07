# ROVERについて

## wringPiインストール
sudo apt install wiringpi

## wiringPiのraspberryPi4向けアップデート方法[[参考]](http://wiringpi.com/wiringpi-updated-to-2-52-for-the-raspberry-pi-4b/)

gpio version 2.52がインストールされていればアップデートが必要無い  

・version確認コマンド
> gpio -v  

・アップデートコマンド  
> cd /tmp  
> wget https://project-downloads.drogon.net/wiringpi-latest.deb  
> sudo dpkg -i wiringpi-latest.deb  

## 環境変数の設定(必須)
> cd ./envVar  
> echo  "PRO4_ROVER_DIR_PATH=\"roverディレクトリのパス\"" > pro4_rover_path.path  
> source ./addEnv.sh  

再起動ごとに必要

## 環境変数の設定(永続)
/etc/profileに環境変数を追加する
PRO4_ROVER_DIR_PATH="roverディレクトリのパス"

USBにF9P接続  
*方位センサーのピンを追加すること  

## f9p
rtklibのstr2strを利用してf9pによるRTK法の結果をTCPで受信する。  
RTK法の結果のTCP配信はv2.4.3でなければできない可能性が高い。[[参考]](https://github.com/tomojitakasu/RTKLIB/issues/573)  
f9pの使い方については[詳細](./src/main/f9p/f9p.md)を確認
  
## システム構成図
![システム構成図](./doc/img/now.png)
  
## ROVERシステムとは
複数のプログラムを同時に動作させることで動作するシステムである。  
現在のROVERシステムはRTK法や方位センサを利用しながら、受信したデータに従ってモータドライバを操作するものである。  

## プログラムについて
./src/main内のプログラムはROVERシステムを動作させるために必須。  
./src/subはROVERの動作を補助するためのもののため、実行しなくてもROVERシステムは動作する。  
各プログラムは基本的に./src/main/rover/rover.out実行後でなければ、動作しない。  
./src/subディレクトリ内のプログラムのsendTypeオプションは送信するtcpStruct構造体の形式を決定する  
基本的にmakeでビルドなどを行う  
### ./src/main/main
> cd ./src/main/rover  
> make  
>  sudo -E ./rover.out  

-Eオプションは環境変数をsudoでも利用するために必要。(例
:sudo -E ./rover.out)  

### ./src/sub/consoleBase
> cd ./src/sub/consoleBase  
> make  
> ./consoleBase.out myTcpServerAddress sendType  

### ./src/sub/consolePattern
> cd ./src/sub/consolePattern  
> make
> ./consolePattern.out myTcpServerAddress sendType  

### ./src/sub/myMonitorTcp
> cd ./src/sub/myMonitorTcp  
> make  
> myMonitorTcp.out myTcpServerAddress  
