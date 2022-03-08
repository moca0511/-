# -*- coding: utf-8 -*-
#ローバサーバ間tcp通信　サーバ側プログラム
#tcp_server_start.shから起動
import socket
import pickle
import sys
import json
import threading, time
import subprocess



end_flag=0	#クライアント切断検知用
#送信タスク　fifoに書き込まれた内容をクライアントに送信
def send_json(cmd, connect):
	global end_flag
	try:
		while True:
			json_str=str(subprocess.check_output( cmd,shell=True))[2:-3]
			json_data = json.loads(json_str)
			print("send : " + json_str)
			msg = pickle.dumps(json_data)
			connect.send(msg)
			if end_flag==1:
				print("endflag=1\n")
				break
	#送信失敗時　クライアント切断処理
	except:
		print("send_error\n")
		end_flag=1
		exit(1)
#受信タスク　クライアントから受信(タイムアウト0.1s)
def recv_json(connect,json_path):
	global end_flag
	BUFFER_SIZE = 1024
	while True:
		try:
			connect.settimeout(0.1)
			msg=connect.recv(BUFFER_SIZE)
			d = pickle.loads(msg)
			print(json.dumps(d, indent=4))
			with open(json_path, 'w') as f:
				json.dump(d, f, ensure_ascii=False, indent=4)
		#受信が0.1s無ければ　クライアント接続情報確認
		except socket.timeout:
			if end_flag==1:
				print("end_flag=1\n")
				break
			else:
				continue
		#json以外が受信or何かしら異常あれば　切断されたと判断
		except:
			print("recv_error\n")
			end_flag=1
			exit(1)
	print("recv_end\n")
				

def main():
	global end_flag
	#arg[1]=connection ip ,[2]=connection port [3]=res json path
	args=sys.argv

	if len(args)!=5:
		print("tcp_server.py ip port path/send.fifo path/recv.json")
		sys.exit(1)


	IP = args[1]
	PORT = args[2]
	FIFO_PATH=args[3]
	JSON_PATH = args[4]

	cmd = "cat " + FIFO_PATH	#fifo読み取りコマンド
	try:
		while True:
			end_flag=0
			#クライアント接続待ち
			with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
				s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
				s.bind((IP, int(PORT)))
				s.listen(1)
				(connection, client) = s.accept()
				print('Client connected', client)
				#送受信を同時にするために別タスクで実行
				thread1 = threading.Thread(target=send_json, args=(cmd, connection,))  # Initialize
				thread2 = threading.Thread(target=recv_json, args=(connection, JSON_PATH,))
				thread1.start() # Start
				thread2.start()
				thread1.join()
				thread2.join()
	except:
		exit(1)
		

if __name__ == "__main__":
	main()
