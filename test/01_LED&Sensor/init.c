
#include "init.h"
#include "static_parameters.h"
#include "iodefine.h"
extern void init_usb(void);

void init_clock(void)
{
	SYSTEM.PRCR.WORD = 0xa50b;			//クロックソース選択の保護の解除

	SYSTEM.PLLCR.WORD = 0x0F00;			/* PLL 逓倍×16 入力1分周 (12.000MHz * 16 = 192MHz)*/
	SYSTEM.PLLCR2.BYTE = 0x00;			/* PLL ENABLE */
	
	SYSTEM.PLLWTCR.BYTE = 0x0F;			/* 4194304cycle(Default) */
	
	
	// ICK   : 192/2 = 96MHz 			// システムクロック CPU DMAC DTC ROM RAM
	// PCLKA : 192/2 = 96MHz 			// 周辺モジュールクロックA ETHERC、EDMAC、DEU
	// PCLKB : 192/4 = 48MHz 			// 周辺モジュールクロックB 上記以外 PCLKB=PCLK
/*	
	SYSTEM.SCKCR.BIT.FCK=0x02;			//FCLK MAX 50MHz  192/4
	SYSTEM.SCKCR.BIT.ICK=0x01;			//ICLK MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PSTOP1=0x01;			//BCLK 出力停止
	SYSTEM.SCKCR.BIT.PSTOP0=0x01;			//SDCLK 出力停止
	SYSTEM.SCKCR.BIT.BCK=0x02;			//BCLK MAX 100MHz ICLK以下にする必要がある192/4
	SYSTEM.SCKCR.BIT.PCKA=0x01;			//PCLKA MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PCKB=0x02;			//PCLKB MAX 50MHz 192/4
	//上記の設定では正しくclock設定ができないため下記のように一括で設定すること
*/
	SYSTEM.SCKCR.LONG = 0x21C21211;			//FCK1/4 ICK1/2 BCLK停止 SDCLK停止 BCK1/4 PCLKA1/2 PCLKB1/4
/*
	SYSTEM.SCKCR2.BIT.UCK=0x03;			//UCLK MAX 48MHz 192/4
	SYSTEM.SCKCR2.BIT.IEBCK=0x02;			//IECLK MAX 50MHz 192/4
*/
	SYSTEM.SCKCR2.WORD = 0x0032;			/* UCLK1/4 IEBCK1/4 */
	SYSTEM.BCKCR.BYTE = 0x01;			/* BCLK = 1/2 */

	SYSTEM.SCKCR3.WORD = 0x0400;			//PLL回路選択

}

void init_io(void)
{
	//LED
	PORTB.PDR.BIT.B0 = IO_OUT;//LED0 Blue
	PORTA.PDR.BIT.B0 = IO_OUT;//LED1 Blue
	PORTB.PDR.BIT.B1 = IO_OUT;//LED2 Blue
	PORT0.PDR.BIT.B5 = IO_OUT;//LED3 Blue

	//Sensor用LED
	PORTB.PDR.BIT.B6 = IO_OUT;//S_LED

	//MOT_POWER
	//PORT1.PDR.BIT.B5 = IO_OUT;//motor Enable
	PORTB.PDR.BIT.B1 = 1;//motor SLEEP (STBY)

	//MOT_CWCCW
	PORTA.PDR.BIT.B0 = 1;//Rmotor PH RIN1
	PORTA.PDR.BIT.B4 = 1;//Rmotor EN RIN2
	PORTB.PDR.BIT.B0 = 1;//Lmotor PH LIN1
	PORTA.PDR.BIT.B6 = 1;//Lmotor EN LIN2

	//Buzzerのポートの初期化
	DISABLE_BUZZER;	
	PORTB.PDR.BIT.B3 = IO_OUT;
	//DISABLE_BUZZER;

	MPC.PWPR.BIT.B0WI=0;
	MPC.PWPR.BIT.PFSWE=1;
	MPC.PB3PFS.BIT.PSEL=1;	//MTIOC0A
	MPC.PC6PFS.BIT.PSEL=1;	//PWM_L R MTIOC3C
	MPC.PE2PFS.BIT.PSEL=1;	//PWM_R L MTIOC4A

	MPC.PE0PFS.BIT.ASEL=1;	//A/D 電源
	MPC.PE1PFS.BIT.ASEL=1;	//A/D SEN_FL
	MPC.P44PFS.BIT.ASEL=1;	//A/D SEN_FR
	MPC.P46PFS.BIT.ASEL=1;	//A/D SEN_L
	MPC.P42PFS.BIT.ASEL=1;	//A/D SEN_R

	//エンコーダ
	MPC.PC2PFS.BIT.PSEL = 3;	//TCLKA 00011b chaB
	MPC.P15PFS.BIT.PSEL = 4;	//TCLKB 00100b chaA
	MPC.PC4PFS.BIT.PSEL = 2;	//MTCLKC 00010b chaB
	MPC.PC5PFS.BIT.PSEL = 2;	//MTCLKD 00010b chaA

	MPC.PWPR.BYTE=0x80;

	PORTB.PMR.BIT.B3=1;	//SP PWM
	PORTC.PMR.BIT.B6=1;	//PWM_L
	PORTE.PMR.BIT.B2=1;	//PWM_R

	PORTE.PMR.BIT.B0=1;	//A/D
	PORTE.PMR.BIT.B1=1;	//A/D
	PORT4.PMR.BIT.B4=1;	//A/D
	PORT4.PMR.BIT.B6=1;	//A/D
	PORT4.PMR.BIT.B2=1;	//A/D

	//エンコーダ
	PORTC.PMR.BIT.B2 = 1;
	PORT1.PMR.BIT.B5 = 1;
	PORTC.PMR.BIT.B4 = 1;
	PORTC.PMR.BIT.B5 = 1;
}

void init_cmt(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(CMT0) = 0;
	MSTP(CMT1) = 0;
	SYSTEM.PRCR.WORD = 0xA500;	

	//CMT0は制御割り込み用タイマとして使用
	CMT0.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT0.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT0.CMCNT=0;		//カウンターのクリア
	CMT0.CMCOR=1500-1;	//1kHz

	IEN(CMT0,CMI0) = 1;	//割り込み要求を許可
	IPR(CMT0,CMI0) = 15;	//割り込み優先度 15が最高
	IR(CMT0,CMI0)=0;	//割り込みステータフラグをクリア

	//CMT1はセンサー制御用タイマとして使用
	CMT1.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT1.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT1.CMCNT=0;		//カウンターのクリア
	CMT1.CMCOR=(1500/4)-1;	//4kHz

	IEN(CMT1,CMI1) = 1;	//割り込み要求を許可 
	IPR(CMT1,CMI1) = 14;	//割り込み優先度を次点に設定
	IR(CMT1,CMI1)=0;	//割り込みステータフラグをクリア

	CMT.CMSTR0.BIT.STR0=1;	//カウントスタート
	CMT.CMSTR0.BIT.STR1=1;	//カウントスタート
}

void init_mtu(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(MTU) = 0;			//MTUモジュールON
	SYSTEM.PRCR.WORD = 0xA500;	
	MTU.TSTR.BYTE=0;		//タイマ動作ストップ

	//スピーカー用MTU
	MTU0.TCR.BIT.CCLR=2;	//PWM TGRBのコンペアマッチでTCNTクリア 
	MTU0.TCR.BIT.TPSC=1;	//PCLK/4 12MHz
	MTU0.TMDR.BIT.MD=2;	//PWM1
	MTU0.TIORH.BIT.IOA=5;	//コンベアマッチてlow初期はhigh
	MTU0.TIORH.BIT.IOB=2;	//コンベアマッチでhigh
	MTU0.TGRA = 6000;	//1kHz
	MTU0.TGRB = (12000-1);

	//右モータ用MTU設定
	MTU3.TCR.BIT.TPSC=1;	//PCLK/4 12MHz
	MTU3.TCR.BIT.CCLR=1;	//PWM TGRAのコンペアマッチでTCNTクリア
	MTU3.TIORH.BIT.IOA=1;	//初期出力0ンペアマッチ0出力
	MTU3.TIORH.BIT.IOB=2;	//初期出力0コンペアマッチ1出力
	MTU3.TGRA = 12000;
	MTU3.TGRB = 50;
	MTU3.TGRC = 48000;
	MTU3.TMDR.BIT.MD=2;		//PWM1
	MTU3.TMDR.BIT.BFA = 1;	//バッファーモードに設定
	MTU3.TIER.BIT.TGIEB = 1;//GRBコンペアマッチでの割り込み許可
	IEN(MTU3,TGIB3) = 1;	//割り込み要求を許可 
	IPR(MTU3,TGIB3) = 13;	//割り込み優先度を次点に設定
	IR(MTU3,TGIB3)=0;		//割り込みステータフラグをクリア

	//左モータ用MTU設定
	MTU.TOER.BIT.OE4A=1;	//MTU出力端子を出力許可する
	MTU4.TCR.BIT.TPSC=1;	//PCLK/4 12MHz
	MTU4.TCR.BIT.CCLR=1;	//PWM TGRAのコンペアマッチでTCNTクリア
	MTU4.TIORH.BIT.IOA=1;	//初期出力0ンペアマッチ0出力
	MTU4.TIORH.BIT.IOB=2;	//初期出力0コンペアマッチ1出力
	MTU4.TGRA = 12000;
	MTU4.TGRB = 50;
	MTU4.TGRC = 48000;
	MTU4.TMDR.BIT.MD=2;		//PWM1
	MTU4.TMDR.BIT.BFA = 1;	//バッファーモードに設定
	MTU4.TIER.BIT.TGIEB = 1;//GRBコンペアマッチでの割り込み許可
	IEN(MTU4,TGIB4) = 1;	//割り込み要求を許可 
	IPR(MTU4,TGIB4) = 12;	//割り込み優先度を次点に設定
	IR(MTU4,TGIB4)=0;		//割り込みステータフラグをクリア

	MTU.TSTR.BIT.CST0 = 0;	//タイマストップ
	MTU.TSTR.BIT.CST3 = 0;	//タイマストップ
	MTU.TSTR.BIT.CST4 = 0;	//タイマストップ
}

void init_adc(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(S12AD) = 0;
	SYSTEM.PRCR.WORD = 0xA500;

	S12AD.ADCER.BIT.ADRFMT=0;//右づめ
	S12AD.ADCSR.BIT.CKS=0x03;//PCLKの分周なし
}

void init_all(void)
{
	init_clock();		//CPUの動作周波数を設定
	init_io();			//I/O(Input / Output)ポートを設定
	init_cmt();			//CMT(Compare Match Timer)を設定(センサ割り込み、時間待ち用。)
	init_mtu();			//MTU(Multi Function Timer Pulse Unit)
	init_adc();			//ADC(Analog Digital Converter)初期化
	init_usb();
}
