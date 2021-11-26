# install
## ngrok  
$ wget https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-arm.zip
$ unzip ngrok-stable-linux-arm.zip
$ sudo mv ngrok /usr/local/bin/

## slackBot
pip3 install slackbot  
HubotをAppからslackに追加する

## slackToken_user
- slackへのpythonを使った読み書きの手順  
1. スラックのチャンネルへのメッセージのやり取りを行うトークンの取得（「slack送受信用Token取得方法」参考）  
2. アプリの対応チャンネルへの追加  
slackアプリで1で作成したAppを右クリック。  
アプリの詳細を開く  
チャンネルにこのアプリを追加する  
3. チャンネルID取得  
slackアプリで取得したいチャンネルを右クリック  
チャンネル詳細を開く  
下にスクロールでチャンネルIDを取得


## slack送受信用Token取得方法
1. appsの作成  
	1. api管理画面URLにアクセス。  
	2. [Create New App]実行  
	3. [From scratch]選択  
	4. [App Name]に好きなAPP名を入力(後から変更可能？)
	5. [Pick a workspace to develop your app in:]でAppsを作成したいワークスペースを選択。   
		(作成したいワークスペースがない場合は「ログイン方法」を確認。)  
	6. [Create App]でAppsの作成  

1. トークンに対する許可の制御  
    下記の二つそれぞれに[Add an OAuth Scope]から下記の4つの権限を与える。  
    
	### 許可の場所
    - OAuth & Permissions > Scopes > Bot Token Scopes  
    
	### 許可の内容
    - channels:history  
	- channels:read  
	- chat:write
	- chat:write.customize  

2.  トークン発行  
	1. F5などで再度画面を読み込み  
	2. [OAuth Tokens for Your Workspace > Install to Workspace]からトークン発行
	3. [OAuth Tokens for Your Workspace > User OAuth Token] から実際のトークを取得  

# バージョンによる変更  
## slackBotV0.0(タグなし)→slackBotV1.0  
### 追加機能

- 複数のslackBotを同時起動時に、どのbotかの識別機能を付けました。  
botごとにngrokの設定ファイルの参照先を変更するようにした  
sh main_start.sh server	→　serverBot起動  
sh main_start.sh base	→　baseBot起動  
sh main_start.sh rover	→　roverBot起動  
sh main_start.sh	→　deffBot起動  

- crontab起動確認  
baseを起動確認をしているcrontabは下記の通りです  
@reboot sh /home/pi/samba/baseRover/src/main/main_start.sh base 30  

- crontab でのngrok機能
crontabで起動時にngrokやstr2strが実行できなかった。
これを/usr/local/bin/内のファイルを直接指定することで、起動できるようにした。

- slackのコマンド入力時のbot指定方法  
入力コマンドは下記の2パターンを想定していますが、helpを含む一部コマンドは全てのbotが反応します。  
1.コマンド  
2.コマンド[スペース][送信先botのモード]  
利用可能なコマンドの詳細はslackにhelpと書き込むことで確認可能です。  

- ngrok起動待機機能追加
ngorkを起動後に、起動するまで最大10s待機する機能追加

- ngrok起動ファイルをslackボットのモードによって変更されるようした。

- f9pの実装  
baseモード時にf9pによる基準局の送信機能を追加  

- slackbotの投稿に現在のモードを赤字で表示するようにした  
ex) (botMode = `deff`)  
slackには[`]で囲まれた文字を赤字で表示する機能があるのでそれを利用した

## 留意事項  
slackへのngrokURLなどに関する投稿はslackbot/plugins/my_mention.py内のgetNameURL()などをを変更することで変更することができるが、変更した場合はsub/slackRead.pyなどの投稿を読み込むプログラムの方も調整する必要がある。

# おまけ  
## pythonでslackbotがslackに投稿したngrokURLを取得する方法  
sub/configディレクトリのtoken.jsonにslack情報を記載したうえで、sub内のslackRead.pyを使う  
- 例  
$ python3 slackRead.py deff  
nowMode=deff  
slackBot, ngrok = 起動中  
抽出  
serverHttp : http://d420-240b-253-c781-f700-53c0-844f-7a0f-92c5.eu.ngrok.io  
serverHttps : https://6f8d-240b-253-c781-f700-53c0-844f-7a0f-92c5.eu.ngrok.io  
mySSH : tcp://5.tcp.eu.ngrok.io:17825  
tcp1 : tcp://7.tcp.eu.ngrok.io:19584  

