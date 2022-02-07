#ifndef _FIFO_H_
#define _FIFO_H_
#include "tcpServerClient/tcpStruct.h"
/*
server←rover  post json
    ローバ緯度
    ローバ経度
    速度
    方位
    RTK結果
    error
	
server→rover  POST
mode,目標緯度,目標経度
mode: 0=stop  1=forward  2=back  3=right  4=left  5=auto
exp1.ローバ前進  1,0,0
exp2.ローバ停止  0,0,0
exp3.緯度10経度5に移動  5,10,5
*/

#pragma pack(1) //構造体のメンバーの境界整列を制御

typedef struct
{
	double lon;
	double lat;
	int rtk;
	double speed;
	double direction;
	int err;
} type_RtoS; //rover->server

typedef struct
{
	int mode;
	double data1;
	double data2;
} type_StoR; //server->rover

#pragma pack() //構造体のメンバーの境界整列を制御(デフォルトに)

class FIFO_ROVER
{
private:
	tcpStruct readData;
	tcpStruct conv(const type_StoR); //type_StoRからtcpStructにデータを変換
	int fdR;
	int fdW;

public:
	FIFO_ROVER(); //FIFO_MASTER_TO_CLIENT準備、FIFO_CLIENT_TO_MASTERが準備できたことを確認
	//~FIFO_ROVER();							//FIFO_MASTER_TO_CLIENTをクローズ
	void write_fifo_toClient(const type_RtoS); //type_RtoSをクライアントに送信
	int read_fifo_fromClient(tcpStruct *);		 //type_StoRをクライアントから読み込み、tcpStructに変換
};

class FIFO_CLIENT
{
private:
	int fdR;
	int fdW;

public:
	FIFO_CLIENT(); //FIFO_CLIENT_TO_MASTER準備、FIFO_CLIENT_TO_MASTERが準備できたことを確認
	// ~FIFO_CLIENT();						//FIFO_CLIENT_TO_MASTERをクローズ
	void send_toServer(const type_RtoS){};		//サーバに送信
	int read_fromServer(type_StoR *) { return 0; }; //サーバから受信
	
	void write_fifo_toMaster(const type_StoR);	//マスターにデータを送信
	int read_fifo_fromMaster(type_RtoS *);			//masterからデータを受信
};
#endif
