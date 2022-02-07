#ifndef __MY_I2C_PI_H__
#define __MY_I2C_PI_H__
//------------------------------
// 出力ピン設定
//------------------------------
#define PI_OUT_NUM 4
static const int OUT_PIN_TO_PI_ARRAY[PI_OUT_NUM] = {11, 12, 14, 15};
#define PI_OUT_OFF LOW
#define PI_OUT_ON HIGH

void setup_Pi_Out() {
  // put your setup code here, to run once:
  for (int i = 0; i < PI_OUT_NUM; i++)
  {
    pinMode(OUT_PIN_TO_PI_ARRAY[i], OUTPUT);
    digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_OFF);
  }
}


//------------------------------
// データ出力
//------------------------------

#define WALL_ON 1
#define WALL_OFF 0
#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3

// 壁のある無しが記載された配列から、piにデータを送信する
// 引数:壁データのある配列
void outpudFromArray(unsigned char flg[]) {
  for (int i = 0; i < 4; i++) {
    if (flg[i] == WALL_ON)
      digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_ON);
    else
      digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_OFF);
  }
}


// 引数numに従って,引数のArrayにテスト用壁データを作って入れる。
// 引数:num 作りたいデータを挿入(0~7)
// 引数:Array[]作ったデータを入れる(要素数は4)
void makeTestWallFlg(int num, unsigned char Array[]) {
  for (int i = 0; i < 4; i++) {
    if ((num & (1 << i) ) != 0)
      Array[i] = WALL_ON;
    else
      Array[i] = WALL_OFF;
  }
}


#endif
