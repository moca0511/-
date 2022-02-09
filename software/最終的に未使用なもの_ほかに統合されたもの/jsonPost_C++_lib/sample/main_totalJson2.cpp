#include "jsonPost/myJson.h"
/*
main_totalJson.cppを実際の環境に近づけたもの

通信1回に約0.8s必要？
*/

#define JOSN__PARENT_TAG "/form/"

struct READING
{
	int num1;
	double num2;
	double num3;
};

void setJson(MY_JSON *);
void getJson(MY_JSON *, struct READING *);

int main(int argc, char *argv[])
{
	MY_JSON FFF("https://httpbin.org/post");

	//server←rover テストデータセット
	setJson(&FFF);

	//送信データの確認
	std::cout << "送信POSTデータの確認\n"
			  << FFF.getWriteData() << "\n\n";

	//POST送信
	FFF.send();

	//送信結果表示
	std::cout << "受信json表示\n"
			  << FFF.getReadData().dump(4) << "\n\n ";

	//読み込み
	struct READING r;
	getJson(&FFF, &r);
	std::cout
		<< "取得データ表示\n"
		<< r.num1 << ", "
		<< r.num2 << ", "
		<< r.num3 << ", "
		<< FFF.getPastTime() << ", "
		<< "\n\n";

	//2回目以降の送信(送信用データが初期化されていることの確認用)
	for (int i = 0; i < 3; i++)
	{
		FFF.addTagData("forData", i * 10);
		FFF.send();
		std::cout << "sendPostData : " << FFF.getWriteData() << ", result : " << FFF.getTagData(JOSN__PARENT_TAG "forData") << ", needTime: " << FFF.getPastTime() << "\n";
	}
	return 0;
}

//server←rover用データの作成
//テストサーバを変更する時は送信内容も適時修正するべき
void setJson(MY_JSON *j)
{
	const double sendDouble[4] = {1.1, 2.2, 3.3, 4.4};
	const int sendInt[2] = {1, 2};

	/*
	送信テストデータセット
	server←rover  post json
		ローバ緯度(lon)
		ローバ経度(lat)
		速度
		方位
		RTK結果
		error
	*/
	j->addTagData("roverLon", sendDouble[0]);
	j->addTagData("roverLat", sendDouble[1]);
	j->addTagData("speed", sendDouble[2]);
	j->addTagData("direction", sendDouble[3]);
	j->addTagData("RTK", sendInt[0]);
	j->addTagData("err", sendInt[1]);

	/*
	受信テストデータセット
	server→rover  POST
		mode,目標緯度,目標経度
		mode: 0=stop  1=forward  2=back  3=right  4=left  5=auto
		exp1.ローバ前進  1,0,0
		exp2.ローバ停止  0,0,0
		exp3.緯度10経度5に移動  5,10,5
	*/
	const int sendInt2 = 2;
	const double sendDouble2[2] = {5.5, 6.6};
	j->addTagData("mode", sendInt2);
	j->addTagData("lon", sendDouble2[0]);
	j->addTagData("lat", sendDouble2[1]);
}

//post通信の結果から必要なデータをを構造体に書き込む
void getJson(MY_JSON *j, struct READING *r)
{
	r->num1 = j->getTagData(JOSN__PARENT_TAG "mode");
	r->num2 = j->getTagData(JOSN__PARENT_TAG "lat", MY_DOUBLE);
	r->num3 = j->getTagData(JOSN__PARENT_TAG "lon", MY_DOUBLE);
}
