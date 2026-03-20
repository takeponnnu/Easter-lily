/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	
	/*****************************************************************************************
	クロック設定
		
	*****************************************************************************************/
	SYSTEM.PRCR.WORD = 0xa50b;		//クロックソース選択の保護の解除

	SYSTEM.PLLCR.WORD = 0x0F00;		/* PLL 逓倍×16 入力1分周 (12.000MHz * 16 = 192MHz)*/
	SYSTEM.PLLCR2.BYTE = 0x00;		/* PLL ENABLE */
	
	SYSTEM.PLLWTCR.BYTE     = 0x0F;		/* 4194304cycle(Default) */
	
	
	// ICK   : 192/2 = 96MHz 		// システムクロック CPU DMAC DTC ROM RAM
	// PCLKA : 192/2 = 96MHz 		// 周辺モジュールクロックA ETHERC、EDMAC、DEU
	// PCLKB : 192/4 = 48MHz 		// 周辺モジュールクロックB 上記以外 PCLKB=PCLK
/*	
	SYSTEM.SCKCR.BIT.FCK=0x02;		//FCLK MAX 50MHz  192/4
	SYSTEM.SCKCR.BIT.ICK=0x01;		//ICLK MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PSTOP1=0x01;		//BCLK 出力停止
	SYSTEM.SCKCR.BIT.PSTOP0=0x01;		//SDCLK 出力停止
	SYSTEM.SCKCR.BIT.BCK=0x02;		//BCLK MAX 100MHz ICLK以下にする必要がある192/4
	SYSTEM.SCKCR.BIT.PCKA=0x01;		//PCLKA MAX 100MHz 192/2
	SYSTEM.SCKCR.BIT.PCKB=0x02;		//PCLKB MAX 50MHz 192/4
	//上記の設定では正しくclock設定ができないため下記のように一括で設定すること
*/
	SYSTEM.SCKCR.LONG = 0x21C21211;		//FCK1/4 ICK1/2 BCLK停止 SDCLK停止 BCK1/4 PCLKA1/2 PCLKB1/4
/*
	SYSTEM.SCKCR2.BIT.UCK=0x03;		//UCLK MAX 48MHz 192/4
	SYSTEM.SCKCR2.BIT.IEBCK=0x02;		//IECLK MAX 50MHz 192/4
*/
	SYSTEM.SCKCR2.WORD = 0x0032;		/* UCLK1/4 IEBCK1/4 */
	SYSTEM.BCKCR.BYTE = 0x01;		/* BCLK = 1/2 */
	
	SYSTEM.SCKCR3.WORD = 0x0400;		//PLL回路選択


	/*****************************************************************************************
	I/O設定
		LEDの設定	
	*****************************************************************************************/	
	//モータ系ピン設定
	//MOT_POWER
	PORTB.PDR.BIT.B7 = 1;//motor SLEEP (STBY)
	//MOT_CWCCW
	PORT5.PDR.BIT.B5 = 1;//Rmotor PH BIN1
	PORTA.PDR.BIT.B3 = 1;//Rmotor EN BIN2
	PORTC.PDR.BIT.B2 = 1;//Lmotor PH AIN1
	PORTC.PDR.BIT.B4 = 1;//Lmotor EN AIN2
	
	//機能ピン設定	
	MPC.PWPR.BIT.B0WI=0;
	MPC.PWPR.BIT.PFSWE=1;
	MPC.PC6PFS.BIT.PSEL=1;	//PWM_B R MTIOC3C
	MPC.PE2PFS.BIT.PSEL=1;	//PWM_A L MTIOC4A
	MPC.PWPR.BYTE=0x80;
	
	//MTUのイニシャライズ
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(MTU) = 0;//MTUモジュールON
	SYSTEM.PRCR.WORD = 0xA500;	
	
	//ピンや機能設定時にはタイマストップ
	MTU.TSTR.BYTE=0;	//タイマ動作ストップ

	//MTU.TOER.BIT.OE3B=1;
	MTU3.TCR.BIT.TPSC=0;	//PCLK/1 48MHz
	MTU3.TCR.BIT.CCLR=5;	//5TGRCをタイマー周期レジスターのクリアに設定
	MTU3.TIORL.BIT.IOC=2;	//立ち下がり0出力
	MTU3.TIORL.BIT.IOD=1;	//TNCTクリアして立ち上がり1出力
	MTU3.TGRC = (480-1);	//Duty value register
	MTU3.TGRD = (56-1);	//timer period register
	MTU3.TGRA = (480-1);	//never mind
	//MTU3.TMDR.BIT.BFA = 1;	//バッファ動作の設定　A=>C
	MTU3.TMDR.BIT.MD=2;	//PWMモード1
	
	MTU3.TIER.BIT.TGIEA = 1;
	IEN(MTU3,TGIA3) = 1;
	IPR(MTU3,TGIA3) = 13;
	IR(MTU3,TGIA3) =0;
	
	
	MTU.TOER.BIT.OE4A=1;
	//左右モータ用MTU4 PWM1 時定数τ=L/R=17uH/(1.07+0.5+0.3)=110kHz
	MTU4.TCR.BIT.TPSC=0;	//PCLK/1 48MHz
	MTU4.TCR.BIT.CCLR=2;	//PWM TGRBのコンペアマッチでTCNTクリア
	MTU4.TIORH.BIT.IOA=1;	//初期出力0コンペアマッチ1出力
	MTU4.TIORH.BIT.IOB=2;	//初期出力1コンペアマッチ0出力
	MTU4.TGRA = (56-1);	//4以下は動作しない
	MTU4.TGRB = (480-1);	//周期 
	MTU4.TGRC = (56-1);
	MTU4.TMDR.BIT.BFA = 1;	//バッファ動作の設定
	MTU4.TMDR.BIT.MD=2;	//PWMモード1
	
	MTU4.TIER.BIT.TGIEB = 1;
	IEN(MTU4,TGIB4) = 1;
	IPR(MTU4,TGIB4) = 12;
	IR(MTU4,TGIB4) =0;
	
	MTU3.TCNT = 0;
	MTU4.TCNT = 0;
	
	PORTC.PMR.BIT.B6=1;	//PWM_B
	PORTE.PMR.BIT.B2=1;	//PWM_A
	//MTU4.TGRA = 24;		//右
	//MTU4.TGRC = 24;		//左
	MTU.TSTR.BIT.CST3 =1;
	MTU.TSTR.BIT.CST4 =1; 	//カウントスタート4ビット動作
	
	
	PORTB.PODR.BIT.B7 = 1;
	PORT5.PODR.BIT.B5 = 1;	//R_PH BIN1
	PORTA.PODR.BIT.B3 = 0;	//R_PH BIN2
	PORTC.PODR.BIT.B2 = 1;	//L_PH AIN1
	PORTC.PODR.BIT.B4 = 0;	//L_PH AIN2
	
	//MTU4.TCNT = 0;
	while(1);	

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
