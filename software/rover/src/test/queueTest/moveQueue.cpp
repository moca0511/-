
#include <iostream>
#include <queue>

typedef struct
{
	int moterL = 0;
	int moterR = 0;
	int time = 0;
} myMoterData;

using namespace std;

int main(int argc, char const *argv[])
{
	queue<myMoterData> q1;
	myMoterData pushData, popData;
	for (int i = 0; i < 3; i++)
	{
		pushData.moterL = i;
		pushData.moterR = i;
		pushData.time = 0;

		q1.push(pushData);
	}

	for (int i = 0; i < 5; i++)
	{
		cout << "残り : " << q1.size() << ", ";
		if (q1.empty() != 1)
		{
			popData = q1.front();

			cout<< "popData : " << "L : " << popData.moterL << ", R : " << popData.moterR << ", T : " << popData.time;
			q1.pop();
			cout << endl;
		}
		else
		{
			cout << "空になった";

			cout << endl;
		}
	}

	return 0;
}
