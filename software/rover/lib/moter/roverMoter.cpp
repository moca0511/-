#include "moter/roverMoter.h"

bool roverMoterExclusive = false;

roverMoter::roverMoter()
{
	if (roverMoterExclusive)
	{
		printf("%s(%s[%d])", __FILE__, __func__, __LINE__);
		printf(" : 既にroverモータは宣言されています\n");
		exit(1);
	}
	else
	{
		roverMoterExclusive = true;
		L.set(PWM_PIN_L, DIR_PIN_L);
		R.set(PWM_PIN_R, DIR_PIN_R);
	}
}

roverMoter::~roverMoter()
{
	L.stop();
	R.stop();
	roverMoterExclusive = false;
}

//モータの初期設定(停止)
void roverMoter::moterSetup()
{
	//setRotationDirectionで二つのモータに＋値を入力時に前進
	L.setRotationDirection(0);
	R.setRotationDirection(0);
	L.stop();
	R.stop();
}

//　目的:モータの設定から動作の種類を確認
int moter2operation(int mL, int mR)
{
	if ((mL == M_PWM_STOP) && (mR == M_PWM_STOP))
	{
		return M_OPERATION_B;
	}
	else if ((mL == M_PWM_SLEEP) || (mR == M_PWM_SLEEP))
	{
		return M_OPERATION_S;
	}
	else
	{
		if (mL <= M_PWM_STOP)
		{
			if (mR <= M_PWM_STOP)
				return M_OPERATION_MP;
			else
				return M_OPERATION_TR;
		}
		else
		{
			if (mR <= M_PWM_STOP)
				return M_OPERATION_TL;
			else
				return M_OPERATION_MB;
		}
	}
	printf("モータの設定値がおかしいです\n");
	return M_OPERATION_S;
}

void operation2moter(int mOp, MOTER_STRUCT *moter, int time_ms, const int powerMode)
{
	moter->time = time_ms;
	switch (mOp)
	{
	case M_OPERATION_S:
		moter->moterL = M_PWM_SLEEP;
		moter->moterR = M_PWM_SLEEP;
		return;
	case M_OPERATION_B:
		moter->moterL = M_PWM_STOP;
		moter->moterR = M_PWM_STOP;
		return;
	case M_OPERATION_MP:
		moter->moterL = ROTATION_REGULAR_MAX;
		moter->moterR = ROTATION_REGULAR_MAX;
		break;
	case M_OPERATION_MB:
		moter->moterL = ROTATION_INVERSE_MAX;
		moter->moterR = ROTATION_INVERSE_MAX;
		break;
	case M_OPERATION_TR:
		moter->moterL = ROTATION_REGULAR_MAX;
		moter->moterR = ROTATION_INVERSE_MAX;
		break;
	case M_OPERATION_TL:
		moter->moterL = ROTATION_INVERSE_MAX;
		moter->moterR = ROTATION_REGULAR_MAX;
		break;
	default:
		printf("err : operation2moter\n");
		return;
	}
	if (powerMode == POWER_HALF)
	{
		if (moter->moterR == ROTATION_INVERSE_MAX)
			moter->moterR == ROTATION_INVERSE_HALF;
		else
			moter->moterR == ROTATION_REGULAR_HALF;

		if (moter->moterL == ROTATION_INVERSE_MAX)
			moter->moterL == ROTATION_INVERSE_HALF;
		else
			moter->moterL == ROTATION_REGULAR_HALF;
	}
	else if (powerMode == POWER_LOW)
	{
		if (moter->moterR == ROTATION_INVERSE_MAX)
			moter->moterR == ROTATION_INVERSE_LOW;
		else
			moter->moterR == ROTATION_REGULAR_LOW;

		if (moter->moterL == ROTATION_INVERSE_MAX)
			moter->moterL == ROTATION_INVERSE_LOW;
		else
			moter->moterL == ROTATION_REGULAR_LOW;
	}
}
