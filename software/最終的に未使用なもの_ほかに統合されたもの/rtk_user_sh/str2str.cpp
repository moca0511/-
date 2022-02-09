//ライブラリ読み込み
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sstream>
#include <iostream>

//---------------------------
//引数により設定されるモード
//---------------------------
//一つの引数しか設定できない
#define MODE_NULL 0	//引数がない時
#define MODE_C 1	//-c	//str2strがバックグラウンドで動作しているかの確認用
#define MODE_K 2	//-k	//バックグラウンドのstr2strを強制終了
#define MODE_R 3  	//-r	//rover		//str2strで移動局データををtcp配信
#define MODE_RN 7	//-rn	//rover_no	//str2strで移動局データをバックグラウンドでtcp配信
#define MODE_BN 4	//-bn	//base_ntrips	//str2strで基準局データをrtk2go.comなどのNtripsで配信
#define MODE_BT 8 	//-bt	//base_tcp	//str2strで基準局データをtcpで配信
#define MODE_H 5 	//-h	//このプログラムのヘルプを表示
#define MODE_HH 6	//-hh	//str2strのヘルプを表示

//---------------------------


#define CONFIG_STR_MAX 256	//設定ファイルから読み込むときの関数
#define CONFIG_FILE "config_master.txt"	//設定データが格納されているファイル(実行ファイルと同じディレクトリに存在する。)

#define TEMP_STR_MAX 500	//複数のパラメータを組み合わせるときの中間配列

#define APP_PATH "/RTKLIB/app/str2str/gcc/str2str"	//RTKLIB内におけるstr2strのパス

char app[] = "echo";
char command1[] = "test";
char printBuff[300];


// ヘルプを表示させた時に出力される内容
const char *help[] = {
	"やりたいこと",
	"・プロセスにstr2strがあるかを確認	:-c",
	"・str2strのプロセスをキル	:-k",
	"・roverの収集＆配信	:-r",
	"・baseの配信		:-b",
	"・helpの表示		:-h",
	"・アプリのヘルプ	:-hh"};

//---------------------------
//クラス宣言
//---------------------------
class parametor
{
private:
	int configRoad(const char parametor_name[]);

public:
	parametor(void){};
	parametor(const char str[]);
	~parametor() { free(parametor_data); };
	char *parametor_data;
	int data_size;
	char confName[CONFIG_STR_MAX] = "";
	void setConfName(const char name[]) { strcpy(confName, name); };
	void myCopy(char read_data[]);
	void printData() { printf("%s\t:\t%d\t:\t%s\n", confName, data_size, parametor_data); };
};

/*
configから読み込むデータ
*/
parametor masterDirPath("masterDirPath");
parametor logDirPath("logDirPath");
parametor usbSpeed("usbSpeed");
parametor tcpPortRawRover("tcpPortRawRover");
parametor tcpPortRawBase("tcpPortRawBase");
parametor myMountName("myMountName");
parametor myMountPassword("myMountPassword");
//char baseMountName[];

/*
補助データ
*/
parametor nowTime;	//ログディレクトリ作成用

/*
連結結果格納
*/
parametor app_path;
parametor outOption_Ntrips;
parametor outOption_Tcp_Rover;
parametor outOption_Tcp_Base;
parametor outOption_File[2];
parametor inOption_Usb;

//---------------------------
//関数宣言
//---------------------------
void printhelp(void);

void makeLogDir(parametor *now);
void makeIn_Usb(parametor *now);
void makeOut_Ntrips(parametor *now);
void makeOut_TcpSvr_RoverRaw(parametor *now);
void makeOut_TcpSvr_BaseRaw(parametor *now);
void makeOut_FileRaw(parametor *now, int num);
void makeAppPath(parametor *now);
//---------------------------

int main(int argc, char *argv[])
{
	masterDirPath.printData();
	logDirPath.printData();
	usbSpeed.printData();
	tcpPortRawRover.printData();
	myMountName.printData();
	myMountPassword.printData();

	makeLogDir(&nowTime);

	makeIn_Usb(&inOption_Usb);

	makeOut_Ntrips(&outOption_Ntrips);
	makeOut_TcpSvr_RoverRaw(&outOption_Tcp_Rover);
	makeOut_TcpSvr_BaseRaw(&outOption_Tcp_Base);
	makeOut_FileRaw(&outOption_File[0], 0);
	makeOut_FileRaw(&outOption_File[1], 1);
	makeAppPath(&app_path);

	app_path.printData();
	nowTime.printData();
	inOption_Usb.printData();
	outOption_Ntrips.printData();
	outOption_Tcp_Rover.printData();
	outOption_Tcp_Base.printData();
	outOption_File[0].printData();
	outOption_File[1].printData();

	//------------------------------
	//引数の読み込みと短い処理
	//------------------------------
	int mode = MODE_NULL;
	if (argc == 1)
	{
		printf("引数が足りません\n");
	}
	else if (argc == 2)
	{
		if (strcmp("-h", argv[1]) == 0)
		{
			mode = MODE_H;
			printhelp();
			return 0;
		}
		else if (strcmp("-c", argv[1]) == 0)
		{
			system("ps aux | grep str2str/gcc/str2str");
			mode = MODE_C;
			return 0;
		}
		else if (strcmp("-k", argv[1]) == 0)
		{
			system("pkill -KILL -f str2str");
			mode = MODE_K;
			return 0;
		}
		else if (strcmp("-r", argv[1]) == 0)
			mode = MODE_R;
		else if (strcmp("-rn", argv[1]) == 0)
			mode = MODE_RN;
		else if (strcmp("-bn", argv[1]) == 0)
		{
			mode = MODE_BN;
		}
		else if (strcmp("-bt", argv[1]) == 0)
		{
			mode = MODE_BT;
		}
		else if (strcmp("-hh", argv[1]) == 0)
		{
			mode = MODE_HH;
			sprintf(printBuff, "%s -h", app_path);
			system(printBuff);
			return 0;
		}
		else
		{
			printf("引数が違います\n");
		}
	}
	else
	{
		printf("引数が多すぎます\n");
	}

	//------------------------------
	//ster2strを使ったデータ配信
	//------------------------------
	std::stringstream ss;
	std::string cmd;
	switch (mode)
	{
	case MODE_R: //rover
	case MODE_RN:
	case MODE_BN: //base
	case MODE_BT:
		//ログ用ディレクトリ作成
		//makeLogDir();
		ss << app_path.parametor_data << std::string(" -in ") << inOption_Usb.parametor_data;
		switch (mode)
		{
		case MODE_R: //rover
			ss << std::string(" -out ") << outOption_Tcp_Rover.parametor_data << std::string(" -out ") << outOption_File[0].parametor_data;
			break;
		case MODE_RN:
			ss << std::string(" -out ") << outOption_Tcp_Rover.parametor_data << std::string(" -out ") << outOption_File[0].parametor_data;
			ss << std::string("  > /dev/null 2>&1 ");
			break;
		case MODE_BN: //base
			ss << std::string(" -out ") << outOption_Ntrips.parametor_data << std::string(" -out ") << outOption_File[1].parametor_data;
			break;
		case MODE_BT:
			ss << std::string(" -out ") << outOption_Tcp_Base.parametor_data << std::string(" -out ") << outOption_File[1].parametor_data;
			break;
		default:
			break;
		}
		cmd = ss.str();
		printf("\n");
		printf("%s\n", cmd.c_str());
		printf("\n"); 
		system(cmd.c_str());
		return 0;
	default:
		break;
	}
	//------------------------------

	return 0;
}

//ヘルプ出力
void printhelp(void)
{
	int i;
	for (i = 0; i < (int)(sizeof(help) / sizeof(*help)); i++)
		fprintf(stderr, "%s\n", help[i]);
	exit(0);
}

//------------------------------------------------------------------------------------------
//parametorクラスの関数
//------------------------------------------------------------------------------------------

//引数のパラメータのデータを設定ファイルから読み込む
//引数:読み込むパラメータ名
parametor::parametor(const char str[])
{
	strcpy(confName, str);
	configRoad(str);
}

//配列のデータをパラメータのデータとして格納
//引数:格納したいパラメータのデータ
void parametor::myCopy(char read_data[])
{
	data_size = strlen(read_data) + 1;
	parametor_data = (char *)malloc(data_size * sizeof(char));
	strcpy(parametor_data, read_data);
}

//設定ファイルからパラメータ名=に続く文字列を改行コードか#が来るまで読み込み続ける
//引数:読み込みたいパラメータ名
//戻り値：-1=システムエラー、-3=パラメータ名が存在しない、0=読み込み成功
//参考：http://nex2t.blog.shinobi.jp/c%E8%A8%80%E8%AA%9E/%E8%A8%AD%E5%AE%9A%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%81%8B%E3%82%89%E7%89%B9%E5%AE%9A%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%82%92%E6%96%87%E5%AD%97%E5%88%97%E3%81%A7%E5%8F%96%E5%BE%97-c%E8%A8%80%E8%AA%9E-
//オリジナルと違い、#以降のパラメータは読み込まない
int parametor::configRoad(const char parametor_name[])
{
	int i = 0, j = 0;
	char str[CONFIG_STR_MAX], param[CONFIG_STR_MAX];
	FILE *fin;
	if ((fin = fopen(CONFIG_FILE, "r")) == NULL)
	{
		printf("fin error:[%s]\n", CONFIG_FILE);
		return -1; /* system error */
	}

	for (;;)
	{
		if (fgets(str, CONFIG_STR_MAX, fin) == NULL)
		{
			/* EOF */
			fclose(fin);
			return -3; /* not found keyword */
		}
		if (!strncmp(str, parametor_name, strlen(parametor_name)))
		{
			while (str[i++] != '=')
			{
				;
			}
			while (str[i] != '\n' && str[i] != '#')
			{
				param[j++] = str[i++];
			}
			param[j] = '\0';
			//printf("param:[%s]\n", param);
			j++;
			//領域確保
			parametor_data = (char *)malloc(j * sizeof(char));
			data_size = j;

			strcpy(parametor_data, param);
			fclose(fin);
			return 0;
		}
	}
	fclose(fin);
	return 0;
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
//　他クラスの格納データから必要なパラメータを作成する関数
//------------------------------------------------------------------------------------------

//ログを格納するディレクトリ名用の時間を取得し、パラメータ名とパラメータデータを格納
//引数:データを格納するparametorクラス
void makeLogDir(parametor *now)
{
	char parametor_data[TEMP_STR_MAX] = "-";

	time_t t;
	struct tm *utcTm; //time構造体

	/* UTC／協定世界時で出力 */
	time(&t);			//通算秒の取得
	utcTm = gmtime(&t); //UTC協定世界時を取得
	sprintf(parametor_data, "%04d-%02d-%02d_%02dh%02dm%02ds",
			utcTm->tm_year + 1900,
			utcTm->tm_mon + 1,
			utcTm->tm_mday,
			utcTm->tm_hour,
			utcTm->tm_min,
			utcTm->tm_sec);

	now->myCopy(parametor_data);
	now->setConfName("nowTime\t");
}

//usbSpeedに格納されたデータを使い「シリアル通信読み込み用パラメータ」を引数のクラス内に格納
//引数:データを格納するparametorクラス
void makeIn_Usb(parametor *now)
{
	char parametor_data[TEMP_STR_MAX];
	sprintf(parametor_data, "serial://ttyACM0:%s#ubx", usbSpeed.parametor_data);

	now->myCopy(parametor_data);
	now->setConfName("inUsb\t");
	return;
}

//myMountPassword、myMountNameに格納されたデータを使い「Ntripsに送信用パラメータ」を引数のクラス内に格納
//引数:データを格納するparametorクラス
void makeOut_Ntrips(parametor *now)
{
	char parametor_data[TEMP_STR_MAX];
	sprintf(parametor_data, "ntrips://:%s@rtk2go.com:2101/%s", myMountPassword.parametor_data, myMountName.parametor_data);
	now->myCopy(parametor_data);
	now->setConfName("outNtrips");
}

//tcpPortRawRoverに格納されたデータを使い「移動局の生データのTCP配信用パラメータ」を引数のクラス内に格納
//引数:データを格納するparametorクラス
void makeOut_TcpSvr_RoverRaw(parametor *now)
{
	char parametor_data[TEMP_STR_MAX];
	sprintf(parametor_data, "tcpsvr://:%s", tcpPortRawRover.parametor_data);
	now->myCopy(parametor_data);
	now->setConfName("outTcpRoverRaw");
}

//tcpPortRawBaeに格納されたデータを使い「移動局の生データのTCP配信用パラメータ」を引数のクラス内に格納
//引数:データを格納するparametorクラス
void makeOut_TcpSvr_BaseRaw(parametor *now)
{
	char parametor_data[TEMP_STR_MAX];
	sprintf(parametor_data, "tcpsvr://:%s", tcpPortRawBase.parametor_data);
	now->myCopy(parametor_data);
	now->setConfName("outTcpBaseRaw");
}

//logDirPath、nowTimeに格納されたデータを使い「logデータのファイル格納用パラメータ」を引数のクラス内に格納
//引数:now=データを格納するparametorクラス、num：0=移動局、1=基準局
void makeOut_FileRaw(parametor *now, int num)
{
	char parametor_data[TEMP_STR_MAX];
	char type[2][6] = {"rover", "base"};
	sprintf(parametor_data, "file://%s/%s/%s.txt", logDirPath.parametor_data, nowTime.parametor_data, type[num]);

	now->myCopy(parametor_data);
	now->setConfName("outFileRaw");
}

//masterDirPathに格納されたデータを使い「実行ファイルのパス用パラメータ」を引数のクラス内に格納
//引数:データを格納するparametorクラス
void makeAppPath(parametor *now)
{
	char parametor_data[TEMP_STR_MAX];
	sprintf(parametor_data, "%s%s", masterDirPath.parametor_data, APP_PATH);
	now->myCopy(parametor_data);
	now->setConfName("AppPath\t");
}
//------------------------------------------------------------------------------------------
