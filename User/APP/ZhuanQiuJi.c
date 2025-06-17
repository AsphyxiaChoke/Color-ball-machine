//----------------------------------------------------------------------
// ͨѶ������ 9600
// ���ƻ�������ָ��˵��
//   ����ͷ AA ���� E1 ���� 01 01 (0x5a-ǰ����У���) cd
// 1.������λ
//	AA E1 01 01 cd  ���뷢�͸�λ����������,��һ��ͨѶĬ��Ϊ0xab��
// ��������Ϊ���ķ���
// 2.ϴ�򲢳���
//	AA E2 02 0A c2
//	      |   |
//	      |   ------��4λ����ϴ��Ȧ�� ��3bit ϴ���� ��1-0bit ϴ���ٶ�
//		  ----------�����(0-24)����1-25����
//
// 3.����
//	AA E3 01 01 cb
// 4.����
//	AA E4 01 01 ca
// 5.״̬��ѯ
//	AA E5 01 01 c9
// ״̬��������
//   AA F1 01 00 be
//         |  |
//         |  -------������д��� 0bit ���� 1bit �Ƹ� 2bit ���� 3bit ���� 4bit ϴ�� ����״̬ 5bit æ
//		  |			6bit 7bit ��������һ����00 ������ 01 ϴ�򲢳��� 10 ���� 11 ������
//         ----------��һ�����������ʱ����ʹ�õ������ (ֻҪ��ѯһ��״̬�ı�һ�������)
// ��������ʱ����5MS���� ��ֹ�������ݳ���
// ���ݼ����㷨Ϊ��(data+�����)^0xca
//----------------------------------------------------------------------
#include "includes.h"
#include "stm32h7xx_hal_conf.h"
#include "stm32h7xx_hal_uart.h"
#ifdef __ZHUANGQIANJI__


#define JIXIN_UART huart3

#define BUF_LEN 40
uint8_t USART2_TX_BUF[BUF_LEN];
uint8_t USART2_RX_BUF[BUF_LEN];
uint8_t USART2_RX_BUF1[BUF_LEN];

extern void BackProcess(void);
extern void CoinKeyProc(void);

uint8_t SendLen = 0;
uint8_t SenIndux = 0;
uint8_t USART2_START_RX = 0;
uint8_t usart2_indux = 0;
uint8_t usart2_end = 0;
volatile uint8_t usart2_busy = 0;

uint8_t MiMARam;
uint8_t MaskXOR=0x19;
uint8_t JixinRxData;
#define WAITMACHINETIME 60

// �жϷ�ʽ���գ�
void JiXin_Rx_Byte(UART_HandleTypeDef *huart)
{
	uint8_t Res;
	Res = JixinRxData;
	_XTMSG("0x%02x", Res);
	Res = (Res ^ MaskXOR) - MiMARam;
	if (USART2_START_RX == 0)
	{
		usart2_indux = 0;
		if (Res == 0xaa)
		{
			USART2_RX_BUF[usart2_indux] = Res; // �����յ����ַ����浽������
			USART2_START_RX = 1;
			usart2_indux++;
		}
	}
	else
	{
		USART2_RX_BUF[usart2_indux] = Res; // �����յ����ַ����浽������
		usart2_indux++;
		if (usart2_indux >= 5)
		{
			if (usart2_end == 0)
			{
				memcpy(USART2_RX_BUF1, USART2_RX_BUF, usart2_indux);
				USART2_START_RX = 0;
			}
			usart2_end = 1;
			usart2_indux = 0;
		}
	}
	if (usart2_indux > 40) // ���������,������ָ��ָ�򻺴���׵�ַ
	{
		usart2_indux = 0;
		USART2_START_RX = 0;
		usart2_end = 0;
	}
	HAL_UART_Receive_IT(&JIXIN_UART, (uint8_t *)&JixinRxData, 1);
}

void JiXin_Tx_Byte(UART_HandleTypeDef *huart)
{
	if (huart == &JIXIN_UART)
	{
		if (SenIndux >= (sizeof(USART2_TX_BUF) - 1))
		{
			SenIndux = 0;
			SendLen = 0;
			usart2_busy = 0;
		}
		else if (SenIndux < SendLen)
		{
			HAL_UART_Transmit_IT(&JIXIN_UART, (uint8_t *)&USART2_TX_BUF[SenIndux++], 1);
		}
		else
		{
			SenIndux = 0;
			SendLen = 0;
			usart2_busy = 0;
		}
	}
}
void Init_JiXinUartSet(void)
{
	HAL_UART_RegisterCallback(&JIXIN_UART, HAL_UART_RX_COMPLETE_CB_ID, JiXin_Rx_Byte);
	HAL_UART_RegisterCallback(&JIXIN_UART, HAL_UART_TX_COMPLETE_CB_ID, JiXin_Tx_Byte);
	HAL_UART_Receive_IT(&JIXIN_UART, (uint8_t *)&JixinRxData, 1); // ���������ж�
}
// void Writejixin(uint8_t *buf, uint32_t len)
uint8_t USART2_SendData(uint8_t *pData, uint8_t buflen)
{
	OS_ERR err;
	if (usart2_busy)
		OSTimeDly(1, OS_OPT_TIME_DLY, &err); // ÿ��OSTimeDly(1=20ms)
	memcpy(USART2_TX_BUF, pData, buflen);
	SenIndux = 0;
	SendLen = buflen;
	HAL_UART_Transmit_IT(&JIXIN_UART, (uint8_t *)&USART2_TX_BUF[SenIndux++], 1);
	return 0;
}
//----------------------------------------------------------
// ��ȡ����
// �����ݷ��� 1 �� 0
//----------------------------------------------------------
uint8_t ZhungQiuJiAsk(uint8_t *state)
{
	uint8_t buf[8];
	uint8_t i = 0;
	uint8_t sum = 0;
	if (usart2_end == 1)
	{
		memcpy(buf, USART2_RX_BUF1, 5);
		usart2_end = 0;
		for (i = 0; i < 4; i++)
		{
			sum += buf[i];
		}
		sum = 0x5a - sum;
		if (buf[4] == sum)
		{
			if (buf[1] == 0xf1)
			{ // ��ȷ
				MiMARam = buf[2];
				*state = buf[3] & 0x3f;
				return 1;
			}
			if (buf[1] == 0xf2)
			{
				MiMARam = buf[2];
				*state = buf[3] & 0x3f;
				return buf[3] + 1;
			}
		}
	}
	return 0; // ����
}
//----------------------------------------------------------
// ��������
// 0 0xe1 ��λ
// 1 0xe2 ϴ��
// 2 0xe3 ����
// 3 0xe4 ����
// 4 0xe5 ��ѯ
// 5 0xe6 ����λ�� 	������λ��
// 6 0xe7 ��̬ϴ�򲢳���
// 7 0xe8 ��̬����
// 8 0xe9 ��̬����
// 9 0xea ���ö�̬��λ....
// 10 0xeb ƥ�䵱ǰ��λ
//----------------------------------------------------------
//const uint8_t cmdTab[11] = {0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCa, 0xcb};
const uint8_t cmdTab[11] = {0xe6, 0xe2, 0xe3, 0xe4, 0xe5, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec};
uint8_t SendCmdZhuanQIuJI(uint8_t cmd, uint8_t sheqiu)
{
	uint8_t buf[15], ballwei[10], ballcnt;
	uint8_t sum = 0, pai, i;
	memset(buf, 0, 5 * sizeof(uint8_t));
	buf[0] = 0xaa;
	buf[1] = cmdTab[cmd];
	buf[2] = 0x01;
	buf[3] = 0x01;
	switch (cmd)
	{
	case 0: // ��λe1---->e6
		MiMARam = 0xab;
		break;
	case 1: // ϴ��e2
	{
		uint8_t err = 1;
		//pai = Pai[0];
		pai = GamePai;
		pai = (((pai & 0x0f) >= 0x0e) ? 4 : (pai & 0x30) >> 4);
		pai++;
		for (i = 0; i < sizeof(Qiuwei) / sizeof(Qiuwei[0]); i++)
		{
			if (Qiuwei[i] == pai)
			{
				err = 0;
				break;
			}
		}
		if (err)
			return 1;
		buf[2] = i; // ��ʲôλ��ȡ��
	}
	break;
	case 2://����e3
	{
#ifdef __DONG_TIA_QIU__
		Qiuwei[TmpQiuwei] = TmpqiuSe; // ��ԭ���λ�õ���ɫ
#endif
	}
	break;
	case 3://����e4
	break;
	case 4://��ѯ״̬e5
	break;
	case 5: // ����e6---->e7
	{
		buf[2] = sheqiu;
		buf[3] = 0xff;
	}
	break;
	case 6: // �Զ���̬ϴ��e7---->e8
	{
		uint8_t err = 1;
		//pai = Pai[0];
		pai = GamePai;
		pai = (((pai & 0x0f) >= 0x0e) ? 4 : (pai & 0x30) >> 4);
		pai++;
		for (i = 0; i < sizeof(Qiuwei) / sizeof(Qiuwei[0]); i++)
		{
			if (Qiuwei[i] == pai)
			{
				err = 0;
				break;
			}
		}
		if (err)
			return 1;
		buf[2] = i + 1; // ��ʲôλ��ȡ��
	}
	break;
	case 7://��̬����e8---->e9
	break;
	case 8://��̬����e9---->ea
	break;
	case 9://���ö�̬��λea---->eb
	case 10://ƥ�䵱ǰ��λ0xeb---->ec 
		for (i = 0; i < sizeof(Qiuwei) / sizeof(Qiuwei[0]); i++)
		{
			buf[2 + i] = Qiuwei[i];
		}
		break;
	}
	if (cmd < 9)//
	{
		sum = buf[0] + buf[1] + buf[2] + buf[3];
		buf[4] = 0x5a - sum;
		if (cmd)
		{
			for (i = 0; i < 5; i++)
				buf[i] = (buf[i] + MiMARam) ^ MaskXOR; // ����
		}
		usart2_indux = 0;
		USART2_START_RX = 0;
		usart2_end = 0;
		while (USART2_SendData(buf, 5))
			;
	}
	else
	{
		sum = 0x5a;
		for (i = 0; i < 12; i++)
		{
			sum -= buf[i];
			buf[i] = (buf[i] + MiMARam) ^ MaskXOR;
		}
		buf[i] = sum;
		buf[i] = (buf[i] + MiMARam) ^ MaskXOR;
		i++;
		usart2_indux = 0;
		USART2_START_RX = 0;
		usart2_end = 0;
		while (USART2_SendData(buf, i))
			;
	}
	return 0;
}
//--------------------------------------------------------------------
// ���͸�λ
//--------------------------------------------------------------------
#define WAITTIMEM 1
void MachineReSet(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
			SendCmdZhuanQIuJI(MACHINERST, 0);
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			break;
		case 1:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		}
		//OSTimeDly(WAITTIMEM);
		OSTimeDly(WAITTIMEM, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0) // ��оδ����
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
					if (HaveKeyOPEN())
					{
						checkcnt = 0;
						station = 0;
						break;
					}
				}
			}
		}
		else // �������յ�
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
					if (HaveKeyOPEN())
					{
						checkcnt = 0;
						station = 0;
						break;
					}
				}
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 0)
				{
					station = 1;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
				station = 1;
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				if (checkcnt >= 3)
				{
					for (;;)
					{
						BackProcess();
						CoinKeyProc();
						if (HaveKeyOPEN())
						{
							checkcnt = 0;
							station = 0;
							break;
						}
					}
				}
				else
				{
					station = 0;
				}
			}
			else
			{
				if (station == 1)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
}
//--------------------------------------------------------------------
// ���ͳ��� ���󷵻� 1 ��ȷ 0
//--------------------------------------------------------------------
uint8_t MachineOutQiu(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
		case 3:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			if (SendCmdZhuanQIuJI(MACHINEXIQIU, 0))
			{
				return 1; // ��λ����
			}
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			break;
		case 2:
			SendCmdZhuanQIuJI(MACHINERST, 0);
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			station = 0;
			break;
		}
		//OSTimeDly(WAITTIMEM);
		OSTimeDly(WAITTIMEM, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
		}
		else
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
					if (HaveKeyOPEN())
					{
						checkcnt = 0;
						station = 2;
						break;
					}
				}
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 3;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				if (state & 0x18)
				{
					__24C04_FLAG |= S_QIUWEI;
					Qiuwei[0] = 0;
					Qiuwei[1] = 0;
					Qiuwei[2] = 0;
					Qiuwei[3] = 0;
					Qiuwei[4] = 0;
					Qiuwei[5] = 0;
					Qiuwei[6] = 0;
					Qiuwei[7] = 0;
					Qiuwei[8] = 0;
					Qiuwei[9] = 0;
				}
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else if (station == 3)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
	return 0;
}
//--------------------------------------------------------------------
// ����1 ���󷵻� 1 ��ȷ 0
//--------------------------------------------------------------------
uint8_t MachineOpenQiu(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
		case 2:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			SendCmdZhuanQIuJI(MACHINEKAIQIU, 0);
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			break;
		}
		//OSTimeDly(WAITTIMEM);
		OSTimeDly(WAITTIMEM, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
		}
		else
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				if (state & 0x18)
				{
					__24C04_FLAG |= S_QIUWEI;
					Qiuwei[0] = 0;
					Qiuwei[1] = 0;
					Qiuwei[2] = 0;
					Qiuwei[3] = 0;
					Qiuwei[4] = 0;
					Qiuwei[5] = 0;
					Qiuwei[6] = 0;
					Qiuwei[7] = 0;
					Qiuwei[8] = 0;
					Qiuwei[9] = 0;
				}
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else if (station == 2)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
	return 0;
}
//--------------------------------------------------------------------
// ����
//--------------------------------------------------------------------
void MachineCloseQiu(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
		case 2:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			SendCmdZhuanQIuJI(MACHINESHOUQIU, 0);
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			checkcnt++;
			break;
		}
		//OSTimeDly(WAITTIMEM);
		OSTimeDly(WAITTIMEM, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
		}
		else
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				return;
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				if (state & 0x18)
				{
					__24C04_FLAG |= S_QIUWEI;
					Qiuwei[0] = 0;
					Qiuwei[1] = 0;
					Qiuwei[2] = 0;
					Qiuwei[3] = 0;
					Qiuwei[4] = 0;
					Qiuwei[5] = 0;
					Qiuwei[6] = 0;
					Qiuwei[7] = 0;
					Qiuwei[8] = 0;
					Qiuwei[9] = 0;
				}
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else if (station == 2)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
}
//--------------------------------------------------------------------
// ����λ
//--------------------------------------------------------------------
uint8_t MachineSheQiu(uint8_t ball)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, station = 0, checkcnt = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (;;)
	{
		switch (station)
		{
		case 0:
		case 3:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			SendCmdZhuanQIuJI(MACHINESHEQIU, ball);
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			checkcnt++;
			break;
		case 2:
			SendCmdZhuanQIuJI(MACHINERST, ball);
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			checkcnt++;
			break;
		}
		//OSTimeDly(WAITTIMEM);
		OSTimeDly(WAITTIMEM, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
		}
		else
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 3;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				if (checkcnt >= 3)
				{
					for (;;)
					{
						BackProcess();
						CoinKeyProc();
					}
				}
				else
				{
					station = 2;
				}
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else if (station == 3)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
	return 0;
}
//--------------------------------------------------------------------
// ������λ
//--------------------------------------------------------------------
void MachineSaveQiu(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
		case 2:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			SendCmdZhuanQIuJI(MAC_SAVE_QIU, 0);
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			checkcnt++;
			break;
		}
		//OSTimeDly(WAITTIMEM * 5);
		OSTimeDly(WAITTIMEM * 5, OS_OPT_TIME_DLY, &err);
		if (ZhungQiuJiAsk(&state) == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				return;
			}
		}
		else
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (Gamestation & MACHERR06)
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				break;
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else if (station == 2)
				{
					Gamestation &= (~0x1f);
					break;
				}
			}
		}
	}
}
//--------------------------------------------------------------------
// �Ƚ���λ ���󷵻� 1 ��ȷ 0
//--------------------------------------------------------------------
uint8_t MachineCapQiu(void)
{
	OS_ERR err;
	uint32_t wait_time, run_time;
	uint8_t state = 0, checkcnt, station = 0, rev = 0;
	run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
	for (checkcnt = 0;;)
	{
		switch (station)
		{
		case 0:
		case 2:
			SendCmdZhuanQIuJI(MACHINECHECK, 0);
			checkcnt++;
			break;
		case 1:
			if (SendCmdZhuanQIuJI(MAC_CAP_QIU, 0))
			{
				return 1; // ��λ����
			}
			checkcnt++;
			run_time = OSTimeGet(&err) + OS_TICKS_PER_SEC * WAITMACHINETIME;
			break;
		}
		//OSTimeDly(WAITTIMEM * 5);
		OSTimeDly(WAITTIMEM * 5, OS_OPT_TIME_DLY, &err);
		rev = ZhungQiuJiAsk(&state);
		if (rev == 0)
		{
			if (checkcnt >= 3)
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
					if (HaveKeyOPEN())
					{
						checkcnt = 0;
						station = 2;
						break;
					}
				}
			}
		}
		else if (rev == 1)
		{
			checkcnt = 0;
			if (run_time < OSTimeGet(&err))
			{
				Gamestation |= MACHERR06;
				for (;;)
				{
					BackProcess();
					CoinKeyProc();
					if (HaveKeyOPEN())
					{
						checkcnt = 0;
						station = 2;
						break;
					}
				}
			}
			else if (Gamestation & MACHERR06)//Gamestation
			{
				Gamestation &= (~MACHERR06);
			}
			if (state & 0x20) // æ��
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // ��1��
				{
					BackProcess();
					CoinKeyProc();
				}
			}
			else if (state & 0x1f)
			{
				Gamestation |= (state & 0x1f);
				return 1;
			}
			else
			{
				if (station == 0)
				{
					station = 1;
				}
				else
				{
					break;
				}
			}
		}
		else if (rev == 2)
		{
			return 1;
		}
	}
	return 0;
}

#endif
