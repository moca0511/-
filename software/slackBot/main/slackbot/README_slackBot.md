# ディレクトリ構成
slackbot         # プログラムをまとめるディレクトリ。名前はなんでも良い  
├─ run.py        # このプログラムを実行することで、ボットを起動する  
├─ slackbot_settings.py   # botに関する設定を書くファイル  
├─ my_common.py			  # token.jsonの読み込みを行うファイル
├─ token.json			  # tokenやチャンネルIDが記録されている(gitに記録しない)
└─ plugins                # botの機能はこのディレクトリに追加する  
   ├─ __init__.py         # モジュールを示すためのファイル。空で良い  
   └─ my_mention.py       # 機能を各ファイル。任意の名前で良い  
