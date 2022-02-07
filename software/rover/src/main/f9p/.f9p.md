# RTK str2strの利用例
## str2strコマンド例
ネットワーク内のPCから受信した基準局データを移動局にUSBを通して配信して、TCPポート2103からRTK方の結果を配信
> ./str2str -in tcpcli://10.43.0.25:2101  -out serial://ttyACM0:115200#2103  

rtk2goから受信した基準局データを移動局にUSBを通して配信して、TCPポート2103からRTK方の結果を配信
> ./str2str -in ntrip://rtk2go.com:2101/KK-LAB-KCG -out serial://ttyACM0:115200#2103  

ネットワーク内のhostnamePCから受信した基準局データを、TCPポートから転送
> ./str2str -in tcpcli://hostname.local:2101  -out tcpsvr://:2224  

## RTKLIB v243のインストール方法

> git clone -b rtklib_2.4.3 https://github.com/tomojitakasu/RTKLIB.git  
> cd RTKLIB/app/consapp  
> make   
  
## v243におけるRTKのtcpサーバ結果配信の方法に関して
[[参考]](https://github.com/tomojitakasu/RTKLIB/issues/573)を確認

## 受信したデータをログファイルに保存するオプション

-out [file://]path[::T][::+start][::xseppd][::S=swap]

## USB接続先について
f9pはUSB接続を行いttyACM0に接続されている場合のみ、利用可能  
下記のコマンドなどで確認  
> cat /dev/ttyACM*

