# 概要
[モータドライバ](https://www.robotshop.com/jp/ja/cytron-10a-5-30v-dual-channel-dc-motor-driver.html)
と
[モータ](https://www.robotshop.com/jp/ja/12v-dc-motor-12kg-cm-350rpm-w-encoder.html)
を利用したC++システムであり、wiringPiをgpioの操作に利用している

# エンコーダ
モータ1回転での出力パルスは374である。  
割り込みを利用して信号を受診しているが、チャッタリングや信号データを格納する変数のオーバーフローについては考慮していないので注意。  
また、エンコーダを3つ以上接続する場合はソースファイルとヘッダファイルに変更が必要  

# モータドライバ
pwm出力にはLAP方式とSMB方式が存在する。  
  
特徴は下記の通り  
・LAPは〇制御性は高く，×消費電流は多い．また、ブレーキが不可  
・SMBは×制御性は低く，〇消費電流は少ない.また、ブレーキが可能  
[参考URL](https://tattatatakemori.hatenablog.com/entry/2017/07/20/232827) 


SMBではなくLAP方式のPWM制御を行っている。  
よって、ブレーキが実行可能。  

モータドライバの各ピンに入力する信号は下記の通り。  
pwmピンにPWM信号を入力するのではないことに留意する。  
| モータドライバPin    | input Data  |
|:-------------------:|:------------:|
| pwm                 |     High/Low |
| dir                 |   PWM        |


# 注意1
ライブラリのクラスの宣言は下記の順で行うべき。  
(1)がなければ、ピン設定が正常に行わられない。  
また、モータを初期化しなければ、前回のモータ動作をエンコーダが読み始めてしまう。
1. wiringPiSetupGpio()
2. モータ
3. エンコーダ

# 注意2
sampleVideo内の動作動画を見れば、わずかに回転し過ぎているように見える  
しかし、サンプルの動作結果を確認すると、停止命令を出した後にも回転を行いパルスが増加している。  
よって、モータを固定や低速回転、フィードバック制御などを行い、停止命令後にパルスが増加しないようにした結果を見て結果を判断するべき。

# サンプル
## 準備
wiringPiのアップデートが完了したraspberry pi4を用意。
## 配線

| RaspberryPi4    | PIN  |
|:-------------------:|:------------:|
| GPIO12                |	モータドライバ_DIR1 |
| GPIO5                 |	モータドライバ_PWM1	|
| GND					| 	モータドライバ_GND	|
| 5v					|	B+	|
| GND					|	B-	|
| - | - |
| 3.3V					|	エンコーダ_3.3v	|
| GPIO20				|	エンコーダ_A	|
| GPIO18				|	エンコーダ_B	|
| GND					| 	エンコーダ_GND	|
  
| エンコーダ   | モータドライバ  |
|:-------------------:|:------------:|
| MOTER+                |	モータドライバ_M1A |
| MOTER-                 |	モータドライバ_M1B	|

## サンプル１
### サンプル1_概要

動作内容  
1. 半周を低速で正回転  
2. 半周を高速で正回転  
3. パルス情報の初期化  
4. 1週を高速で逆回転  

また、各動作の間に停止と、パウルの表示を行う。

### サンプル1_コマンド

サンプルプログラム
> sh ./lib/makeLib.sh  
> sh ./sample/make.sh  
> sudo ./bin/sample.out  

製作ファイルの消去
> sh ./sample/clean.sh  

### サンプル1_動作結果例

>$ sudo ./bin/sample.out  

>スタートパルス: 0  
>動作終了時パルス:       -188  
>制動動作分パルス:       -196  
>  
>動作終了時パルス:       -375  
>制動動作分パルス:       -391  
>  
>re:スタートパルス:      0  
>動作終了時パルス:       375  
>制動動作分パルス:       390  
>  
>最終パルス:     390  

## サンプル2
### サンプル2_概要
10回転を行う

### サンプル2_コマンド

サンプルプログラム
> sh ./lib/makeLib.sh  
> sh ./sample/make2.sh  
> sudo ./bin/sample2.out  
  
製作ファイルの消去
> sh ./sample/clean.sh  

### サンプル2_動作結果例

>$ sudo ./bin/sample2.out  
  
> スタートパルス: 0  
> 動作終了時パルス:       -3751  
> 制動動作分パルス:       -3757  
>  
> 最終パルス:     -3757  

# wiringPiのraspberryPi4向けアップデート方法[[参考]](http://wiringpi.com/wiringpi-updated-to-2-52-for-the-raspberry-pi-4b/)

gpio version 2.52がインストールされていればアップデートが必要無い  

・version確認コマンド
> gpio -v  

・アップデートコマンド  
> cd /tmp  
> wget https://project-downloads.drogon.net/wiringpi-latest.deb  
> sudo dpkg -i wiringpi-latest.deb  
