
#include "init.h"
#include "static_parameters.h"
#include "iodefine.h"
#include "glob_var.h"
#include "spi.h"
#include "DataFlash.h"
extern void init_usb(void);
extern wait_ms(int wtime);

//extern void USB_PRINTF(char *format, ...);

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
	PORTA.PDR.BIT.B3 = IO_OUT;//LED0 Blue
	PORTC.PDR.BIT.B3 = IO_OUT;//LED1 Blue
	PORTB.PDR.BIT.B7 = IO_OUT;//LED2 Blue
	PORT5.PDR.BIT.B5 = IO_OUT;//LED3 Blue

	//Sensor
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
	//MPC.PC3PFS.BIT.PSEL = 3;	//TCLKB 00011b
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
	//PORTC.PMR.BIT.B3 = 1;
	PORTC.PMR.BIT.B4 = 1;
	PORTC.PMR.BIT.B5 = 1;
}

void init_cmt(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(CMT0) = 0;
	MSTP(CMT1) = 0;
	MSTP(CMT2) = 0;
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
	
	CMT2.CMCR.BIT.CKS=1;	// PCLK/32 1.5MHz
	CMT2.CMCR.BIT.CMIE=1;	//割り込みを許可
	CMT2.CMCNT=0;		//カウンターのクリア
	CMT2.CMCOR=(1500/2)-1;	//2kHz

	IEN(CMT2,CMI2) = 1;	//割り込み要求を許可 
	IPR(CMT2,CMI2) = 13;	//割り込み優先度を次点に設定
	IR(CMT2,CMI2)=0;	//割り込みステータフラグをクリア

	CMT.CMSTR0.BIT.STR0=1;	//カウントスタート
	CMT.CMSTR0.BIT.STR1=1;	//カウントスタート
	CMT.CMSTR1.BIT.STR2=1;	//カウントスタート
	
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
		
	//左モータ用MTU設定
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
	IPR(MTU3,TGIA3) = 12;
	IR(MTU3,TGIA3) =0;
	
	
	MTU.TOER.BIT.OE4A=1;
	//右モータ用MTU4 PWM1 時定数τ=L/R=17uH/(1.07+0.5+0.3)=110kHz
	MTU4.TCR.BIT.TPSC=0;	//PCLK/1 48MHz
	MTU4.TCR.BIT.CCLR=2;	//PWM TGRBのコンペアマッチでTCNTクリア
	MTU4.TIORH.BIT.IOA=1;	//初期出力0コンペアマッチ1出力
	MTU4.TIORH.BIT.IOB=2;	//初期出力1コンペアマッチ0出力
	MTU4.TGRA = (56-1);	//4以下は動作しない
	MTU4.TGRB = (480-1);	//周期 100kHz  Dutyの決定に関与してくる。interrupt参照
	//MTU4.TGRC = (56-1);
	//MTU4.TMDR.BIT.BFA = 1;	//バッファ動作の設定
	MTU4.TMDR.BIT.MD=2;	//PWMモード1
	
	MTU4.TIER.BIT.TGIEB = 1;
	IEN(MTU4,TGIB4) = 1;
	IPR(MTU4,TGIB4) = 11;
	IR(MTU4,TGIB4) =0;
	
	PORTC.PMR.BIT.B6=1;	//PWM_L
	PORTE.PMR.BIT.B2=1;	//PWM_R

	//エンコーダ
	MTU2.TMDR.BIT.MD = 4;		//位相計数モード1
	MTU2.TCR.BIT.CCLR = 0;		//TCNTクリア禁止
	MTU2.TCNT = 0;

	MTU.TSTR.BIT.CST0 = 0;	//タイマスタート
	MTU.TSTR.BIT.CST2 = 1;	//タイマスタート
	MTU.TSTR.BIT.CST3 = 1;	//タイマスタート
	MTU.TSTR.BIT.CST4 = 1;	//タイマスタート
	
	MOT_POWER_OFF;
	MOT_CWCCW_R1  = 0;	//R_PH
	MOT_CWCCW_R2  = 0;
	MOT_CWCCW_L1  = 0;	//L_PH
	MOT_CWCCW_L2  = 0;
	
	
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(TPU1) = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	//ピンや機能設定時にはタイマストップ
	TPUA.TSTR.BYTE = 0;
	
	TPU1.TMDR.BIT.MD = 4;		//位相計数モード1
	TPU1.TCR.BIT.CCLR = 0;		//TCNTクリア禁止
	TPU1.TCNT = 0;
	TPUA.TSTR.BIT.CST1 = 1;	
	
}

void init_adc(void)
{
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(S12AD) = 0;
	SYSTEM.PRCR.WORD = 0xA500;	
	
	S12AD.ADCER.BIT.ADRFMT=0;//右づめ
	S12AD.ADCSR.BIT.CKS=0x03;//PCLKの分周なし
//	S12AD.ADSSTR01.BIT.SST1=20;//Default 20ステート 0.417us 0.4us以上が推奨
}
void init_sci(void){
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(SCI1) = 0;
	SYSTEM.PRCR.WORD = 0xA500;

	SCI1.SCR.BYTE = 0x00;
	while (0x00 != (SCI1.SCR.BYTE & 0xF0));	//割り込み要求が禁止されるまで待つ
	PORT2.PODR.BIT.B6 = 1;			//TXDのDirctionの切り替え後の値をhigh
	PORT2.PDR.BIT.B6 = 1;			//出力に設定
	PORT3.PDR.BIT.B0 = 0;			//入力に設定
	PORT2.PMR.BIT.B6 = 0;			//汎用ポートに設定
	PORT3.PMR.BIT.B0 = 0;			//汎用ポートに設定
	MPC.PWPR.BIT.B0WI  = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P26PFS.BIT.PSEL = 0x0A;		//TXD1
	MPC.P30PFS.BIT.PSEL = 0x0A;		//RXD1
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI  = 1;
	PORT3.PMR.BIT.B0 = 1;			//周辺機能(RXD1)として使用
	SCI1.SCR.BIT.CKE = 0;
	SCI1.SMR.BYTE = 0x00;			//1stopbit parityなし　8bit 調歩同期
	SCI1.SCMR.BYTE = 0xF2;			//S=32clock
	SCI1.SEMR.BYTE = 0x00;
	SCI1.BRR =38;				//@48MHz 38400bps
	SCI1.SCR.BYTE =0x30;			//送信割り込み禁止
	PORT2.PMR.BIT.B6 = 1;			//周辺機能(TXD1)として使用
	SCI1.SCR.BIT.TE = 1;
	SCI1.SCR.BIT.RE = 1; 
}	

void gyro_get_ref(void){
	long i = 0;
	float gyro_ref_temp = 0;
	gyro_ref = 0;
	//ジャイロのリファレンス取得
	for(i = 0; i < 2500; i++){
		gyro_ref_temp += (float)gyro_x_new;
		wait_ms(1);
	}
	gyro_ref = (gyro_ref_temp/2500.0);
	degree = 0;
	wait_ms(100);
}

void init_enc_r(int x){
	int p_enc_r;
	
	if( x == 0 ){
		p_enc_r++;
	
		if( p_enc_r == 4096 ){
			p_enc_r = 0;
			R++;
		}
	}
	else {
		//USB_PRINTF("yomareta\n\r");
		CLR(TPU1,TCI1U) =1;
		p_enc_r--;
		
		if( p_enc_r == -4096 ){
			p_enc_r = 0;
			R--;
		}
	}
		
	enc_r = p_enc_r + 4096*R;
	//TPU1.TCNT = 0;
}

void init_enc_l(void){
	enc_l++;
}

int get_encoder_r(void){
	int count;
	count = 0;
	count = TPU1.TCNT;
	
	//ピンや機能設定時にはタイマストップ
	TPUA.TSTR.BYTE = 0;
	TPU1.TCNT = 0;	//32767;
	TPUA.TSTR.BIT.CST1 = 1;		//TNCTカウントスタート
	
	if( count >= 32767 ){
		count = count - 65536;
	}
	
	return count;
}

int get_encoder_l(void){
	int count;
	count = 0;
	count = MTU2.TCNT;
	
	//ピンや機能設定時にはタイマストップ
	//MTU.TSTR.BYTE = 0;
	MTU2.TCNT = 0;	//32767;
	//MTU.TSTR.BIT.CST2 = 1;		//TNCTカウントスタート
	
	if( count >= 32767 ){
		count = count - 65536;
	}
	count = -1 * count;
	
	return count;
}

void init_parameters(void)
{
			
	sen_r.ref = REF_SEN_R;				//右センサのリファレンス値を初期化
	sen_l.ref = REF_SEN_L;				//左センサのリファレンス値を初期化
	
	sen_r.th_wall = TH_SEN_R;			//右センサの壁有無判断の閾値を初期化
	sen_l.th_wall = TH_SEN_L;			//左センサの壁有無判断の閾値を初期化
	
	sen_fr.th_wall = TH_SEN_FR;			//右前センサの壁有無判断の閾値を初期化
	sen_fl.th_wall = TH_SEN_FL;			//左前センサの壁有無判断の閾値を初期化
	
	sen_r.th_control = CONTH_SEN_R;			//右センサの壁制御かけるか否かの閾値を初期化
	sen_l.th_control = CONTH_SEN_L;			//左センサの壁制御かけるか否かの閾値を初期化
	
	con_wall.kp = CON_WALL_KP/10000.0;			//壁比例制御の比例定数を初期化
}


void init_maze(void)	//迷路情報の初期化
{
	int i,j;
	
	for(i = 0; i < MAZESIZE_X; i++)
	{
		for(j = 0; j < MAZESIZE_Y; j++)
		{
			wall[i][j].north = wall[i][j].east = wall[i][j].south = wall[i][j].west = UNKNOWN;	//迷路の全体がわからない事を設定する
		}
	}
	
	for(i = 0; i < MAZESIZE_X; i++)
	{
		wall[i][0].south = WALL;		//四方の壁を追加する(南)
		wall[i][MAZESIZE_Y-1].north = WALL;	//四方の壁を追加する(北)
	}
	
	for(j = 0; j < MAZESIZE_Y; j++)
	{
		wall[0][i].west = WALL;			//四方の壁を追加する(西)
		wall[MAZESIZE_X-1][i].east = WALL;	//四方の壁を追加する(東)
	}
	
	wall[0][0].east = wall[1][0].west = WALL;	//スタート地点の右の壁を追加する
	
}

void init_all(void)
{
	int i;
	init_clock();			//CPUの動作周波数を設定
	init_io();			//I/O(Input / Output)ポートを設定
	init_mtu();			//MTU(Multi Function Timer Pulse Unit)
	init_adc();			//ADC(Analog Digital Converter)初期化
	init_usb();
	init_spi_gyro();
	init_parameters();	//パラメータの初期化
	init_maze();		//迷路情報の初期化
	
	//Gyro初期設定
	preprocess_spi_gyro_2byte(0x4E1F);		//スリープモード解除
	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_gyro_2byte(0x4F06);		//SPIを有効にする
	for(i = 0; i < 100*1000*10; i++);
	
	//エンコーダスタート
	MTU.TSTR.BIT.CST2 = 1;	//タイマスタート
	TPUA.TSTR.BIT.CST1 = 1;	
	
	init_cmt();
	hw_dflash_init();				// cmtの設定後じゃないとダメみたい
}
