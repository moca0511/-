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
