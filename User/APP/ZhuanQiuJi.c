//----------------------------------------------------------------------
// 通讯波特率 9600
// 控制机器运作指令说明
//   引导头 AA 命令 E1 数据 01 01 (0x5a-前面数校验和) cd
// 1.机器复位
//	AA E1 01 01 cd  明码发送复位命令（清随机数,第一次通讯默认为0xab）
// 以下命令为密文发送
// 2.洗球并出球
//	AA E2 02 0A c2
//	      |   |
//	      |   ------高4位代表洗球圈数 第3bit 洗球方向 第1-0bit 洗球速度
//		  ----------出球号(0-24)代表1-25号球
//
// 3.开球
//	AA E3 01 01 cb
// 4.收球
//	AA E4 01 01 ca
// 5.状态查询
//	AA E5 01 01 c9
// 状态返回命令
//   AA F1 01 00 be
//         |  |
//         |  -------电机运行错误 0bit 顶球 1bit 移盖 2bit 翻球 3bit 球门 4bit 洗球 机器状态 5bit 忙
//		  |			6bit 7bit 机器在那一步（00 收完球 01 洗球并出球 10 开球 11 保留）
//         ----------下一条命令发送数据时加密使用的随机数 (只要查询一次状态改变一次随机数)
// 发送命令时间间隔5MS以上 防止接收数据出错
// 数据加密算法为：(data+随机数)^0xca
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

// 中断方式接收，
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
			USART2_RX_BUF[usart2_indux] = Res; // 将接收到的字符串存到缓存中
			USART2_START_RX = 1;
			usart2_indux++;
		}
	}
	else
	{
		USART2_RX_BUF[usart2_indux] = Res; // 将接收到的字符串存到缓存中
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
	if (usart2_indux > 40) // 如果缓存满,将缓存指针指向缓存的首地址
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
	HAL_UART_Receive_IT(&JIXIN_UART, (uint8_t *)&JixinRxData, 1); // 启动接收中断
}
// void Writejixin(uint8_t *buf, uint32_t len)
uint8_t USART2_SendData(uint8_t *pData, uint8_t buflen)
{
	OS_ERR err;
	if (usart2_busy)
		OSTimeDly(1, OS_OPT_TIME_DLY, &err); // 每个OSTimeDly(1=20ms)
	memcpy(USART2_TX_BUF, pData, buflen);
	SenIndux = 0;
	SendLen = buflen;
	HAL_UART_Transmit_IT(&JIXIN_UART, (uint8_t *)&USART2_TX_BUF[SenIndux++], 1);
	return 0;
}
//----------------------------------------------------------
// 收取数据
// 有数据返回 1 无 0
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
			{ // 正确
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
	return 0; // 出错
}
//----------------------------------------------------------
// 发送命令
// 0 0xe1 复位
// 1 0xe2 洗球
// 2 0xe3 开球
// 3 0xe4 收球
// 4 0xe5 查询
// 5 0xe6 出球位置 	设置球位用
// 6 0xe7 动态洗球并出球
// 7 0xe8 动态开球
// 8 0xe9 动态收球
// 9 0xea 设置动态球位....
// 10 0xeb 匹配当前球位
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
	case 0: // 复位e1---->e6
		MiMARam = 0xab;
		break;
	case 1: // 洗球e2
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
		buf[2] = i; // 从什么位置取球
	}
	break;
	case 2://开球e3
	{
#ifdef __DONG_TIA_QIU__
		Qiuwei[TmpQiuwei] = TmpqiuSe; // 还原这个位置的球色
#endif
	}
	break;
	case 3://收球e4
	break;
	case 4://查询状态e5
	break;
	case 5: // 设球e6---->e7
	{
		buf[2] = sheqiu;
		buf[3] = 0xff;
	}
	break;
	case 6: // 自动动态洗球e7---->e8
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
		buf[2] = i + 1; // 从什么位置取球
	}
	break;
	case 7://动态开球e8---->e9
	break;
	case 8://动态收球e9---->ea
	break;
	case 9://设置动态球位ea---->eb
	case 10://匹配当前球位0xeb---->ec 
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
				buf[i] = (buf[i] + MiMARam) ^ MaskXOR; // 加密
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
// 发送复位
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
		if (ZhungQiuJiAsk(&state) == 0) // 机芯未连接
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
		else // 有数据收到
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
			if (state & 0x20) // 忙等
			{
				if (station == 0)
				{
					station = 1;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 发送出球 错误返回 1 正确 0
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
				return 1; // 球位错误
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 3;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 开球1 错误返回 1 正确 0
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 收球
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 设球位
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 3;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 保存球位
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
// 比较球位 错误返回 1 正确 0
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
				return 1; // 球位错误
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
			if (state & 0x20) // 忙等
			{
				if (station == 1)
				{
					station = 2;
				}
				for (wait_time = OSTimeGet(&err) + OS_TICKS_PER_SEC / 2; wait_time > OSTimeGet(&err);) // 等1秒
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
