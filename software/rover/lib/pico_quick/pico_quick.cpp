#include "pico_quick.h"

myGpio::~myGpio()
{
	if (setFlg != true)
		return;
	if (_mode == GPIO_MODE_OUT)
		setModeIN();

	fd = open(set_FILENAME(FILE_CONTROL_UNEXPORT).c_str(), O_WRONLY);
	writeGpioNum();
}

void myGpio::setModeIN()
{
	_mode = GPIO_MODE_IN;
	fd = open(set_FILENAME(FILE_CONTROL_DIRECTION, gpioPin).c_str(), O_WRONLY);
	if (fd == -1)
	{
		int number = errno;
		perror("err open to write in");
		printf("error code: %d\n", number);
		printf("errNum=EACCES(13)なら sudoを付けてください\n");
	}
	write(fd, "in", 2);
	close(fd);
}

void myGpio::writeGpioNum()
{
	if (gpioPin >= 10)
		write(fd, std::to_string(gpioPin).c_str(), 2);
	else
		write(fd, std::to_string(gpioPin).c_str(), 1);
	if (close(fd) == -1)
		perror("close err\n");

}

//gpioの初期化
//引数num gpioのgpio番号
//引数mode gpioピンの設定モード(GPIO_MODE_IN,GPIO_MODE_OUTから選択可能)
void myGpio::set(int num, int mode)
{
	setFlg = true;
	gpioPin = num;
	//Enablegpio25

	fd = open(set_FILENAME(FILE_CONTROL_EXPORT).c_str(), O_WRONLY);
	writeGpioNum();

	if (mode == GPIO_MODE_IN)
	{
		setModeIN();
	}
	else
	{
		_mode = GPIO_MODE_OUT;
		//出力時のgpio設定
		//system("ls /sys/class/gpio");
		//system("ls /sys/class/gpio/gpio18");
		//printf("%s\n", set_FILENAME(FILE_CONTROL_DIRECTION, gpioPin).c_str());
		fd = open(set_FILENAME(FILE_CONTROL_DIRECTION, gpioPin).c_str(), O_WRONLY);
		if (fd == -1)
		{
			int number = errno;
			perror("err open");
			printf("error code: %d\n", number);
			printf("errNum=EACCES(13)なら sudoを付けてください\n");
		}
		int e = write(fd, "out", 3);
		if (e != 3)
		{
			int number = errno;
			perror("print error string by perror  ");
			printf("print error code: %d\n", number);
			exit(1);
		}

		close(fd);
		myWrite(0);
	}
}

//typeによってnumPinの設定用ファイルパスを読み出す
//引数num gpioのgpio番号
//引数mode gpioピンの設定モード(FILE_CONTROL_XXXから選択可能)
std::string myGpio::set_FILENAME(int type, int num)
{
	std::string r;
	char tmp[50];
	switch (type)
	{
	case FILE_CONTROL_EXPORT:
		sprintf(tmp, "/sys/class/gpio/export");
		break;
	case FILE_CONTROL_DIRECTION:
		sprintf(tmp, "/sys/class/gpio/gpio%d/direction", num);
		break;
	case FILE_CONTROL_EDGE:
		sprintf(tmp, "/sys/class/gpio/gpio%d/edge", num);
		break;
	case FILE_CONTROL_VALUE:
		sprintf(tmp, "/sys/class/gpio/gpio%d/value", num);
		break;
	case FILE_CONTROL_UNEXPORT:
		sprintf(tmp, "/sys/class/gpio/unexport");
		break;
	case FILE_CONTROL_DIR:
		sprintf(tmp, "/sys/class/gpio/gpio%d", num);
		break;
	default:
		break;
	}
	r = tmp;
	return r;
}

//設定済みgpiopinがら読み込んだ結果を返す
//戻り値:1=読み込んだ結果が1, その他=0
bool myGpio::myRead()
{
	char c;
	if (!setFlg)
		return 0;
	fd = open(set_FILENAME(FILE_CONTROL_VALUE, gpioPin).c_str(), O_RDONLY);
	read(fd, &c, 1);
	close(fd);
	if (c == '1')
		return 1;
	return 0;
}

void myGpio::myWrite(bool num)
{
	if (!setFlg)
		return;
	fd = open(set_FILENAME(FILE_CONTROL_VALUE, gpioPin).c_str(), O_WRONLY);
	if (num == 0)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

	close(fd);
}

//------------------------------
//picoｎpin 接続
//------------------------------

//picoと接続しているgpioのピンを初期設定(PIN_WALL_XXXを参考にする)
pico_quick::pico_quick()
{
	readGpio[WALL_FRONT].set(PIN_WALL_FRONT);
	readGpio[WALL_RIGHT].set(PIN_WALL_RIGHT);
	readGpio[WALL_POST].set(PIN_WALL_POST);
	readGpio[WALL_LEFT].set(PIN_WALL_LEFT);
}

//gpioを読み取り、wallFlgに結果を保存
void pico_quick::getPico()
{
	for (int i = 0; i < 4; i++)
		wallFlg[i] = readGpio[i].myRead();
}
