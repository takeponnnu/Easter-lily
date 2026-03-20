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
#include "portdef.h"
#include "init.h"
#include "glob_var.h"
#include <spi.h>


extern void view_adc(void)	;
extern void USB_PRINTF(char *format, ...);
extern int get_encoder_r(void);
extern int get_encoder_l(void);

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

	init_all();		//各種機能の初期化
	//view_adc();	
	
	//sciのイニシャライズ
	init_sci();	
	//init_usb();
	//画面クリアシーケンス
	//SCI_printf("\x1b[2J");				//クリアスクリーン[CLS]
	//SCI_printf("\x1b[0;0H");			//カーソルを0,0に移動
	
	USB_PRINTF("\x1b[2J");
	USB_PRINTF("\x1b[0;0H");
	
	//spiのイニシャライズ
	//init_spi_gyro();
	
	SYSTEM.MSTPCRA.BIT.MSTPA13 = 0;		//TPU1のモジュールストップ状態解除
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;		//MTU2のモジュールストップ状態解除
	
	//機能ピン設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	
	MPC.PC2PFS.BIT.PSEL = 3;	//TCLKA 00011b chaA
	MPC.PC3PFS.BIT.PSEL = 3;	//TCLKB 00011b
	MPC.PC4PFS.BIT.PSEL = 2;	//MTCLKC 00010b
	MPC.PC5PFS.BIT.PSEL = 2;	//MTCLKD 00010b
	//MPC.P15PFS.BIT.PSEL = 3;	//TIOCB2(TCLKC) 00011b
	//MPC.PA6PFS.BIT.PSEL = 3;	//TIOCA2(TCLKD)
	//MPC.PB5PFS.BIT.PSEL = 3;	//TIOCB4 00011b
	
	PORTC.PMR.BIT.B2 = 1;
	PORTC.PMR.BIT.B3 = 1;
	PORTC.PMR.BIT.B4 = 1;
	PORTC.PMR.BIT.B5 = 1;
	//PORT1.PMR.BIT.B5 = 1;		// P15 => TIOCB2(TCLKC)
	//PORTA.PMR.BIT.B6 = 1;
	
	MPC.PWPR.BYTE = 0x80;
	
	//TPUのイニシャライズ
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(TPU1) = 0;
	SYSTEM.PRCR.WORD = 0xA500;
	
	//ピンや機能設定時にはタイマストップ
	TPUA.TSTR.BYTE = 0;
	
	TPU1.TMDR.BIT.MD = 4;		//位相計数モード1
	TPU1.TCR.BIT.CCLR = 0;		//TCNTクリア禁止
	//TPU1.TCR.BIT.CKEG = 1;		//TCNTは立ち下がりエッジでカウント(多分設定いらない)
	//TPU1.TCR.BIT.TPSC = 0;		//TCNTは内部クロック PCLK/1 でカウント(多分設定いらない)
	//TPU1.TIER.BIT.TGIEA = 1;	//割り込み許可
	//TPU1.TIER.BIT.TCIEU = 1;	//割り込み許可
	TPU1.TCNT = 0;
	//TPU1.TGRA = 0;
	//TPU1.TGRA = 32767;
	
/*	IEN(TPU1,TGI1A) =1;		//割り込み要求許可
	IPR(TPU1,TGI1A) = 9;		//割り込み優先度設定
	IR(TPU1,TGI1A) =0;		//割り込み要求(ステータスフラグ)
	
	EN(TPU1,TCI1U) =1;		//グループ割り込み要求許可
	//IPR(TPU1,TCI1U) = 8;
	IS(TPU1,TCI1U) =0;		//グループ割り込みステータスフラグ
	
	//TPU1.TMDR.BIT.MD = 4;		//位相計数モード1
	//TPU4.TMDR.MD = 5;		//位相計数モード2
*/	

	//ピンや機能設定時にはタイマストップ
	MTU.TSTR.BYTE = 0;
	
	//MTUのイニシャライズ
	SYSTEM.PRCR.WORD = 0xA502;
	MSTP(MTU) = 0;
	SYSTEM.PRCR.WORD = 0xA500;

	MTU2.TMDR.BIT.MD = 4;		//位相計数モード1
	MTU2.TCR.BIT.CCLR = 0;		//TCNTクリア禁止
	MTU2.TCNT = 0;

	TPUA.TSTR.BIT.CST1 = 1;		//TNCTカウントスタート
	MTU.TSTR.BIT.CST2 = 1;		//MTU2.TNCTカウントスタート
	
	//TPU1.TCNT += 11;

	int p_enc_r = 0;
	int p_enc_l = 0;
	while(1){
		p_enc_r = get_encoder_r();
		enc_r += p_enc_r;
		
		p_enc_l = get_encoder_l();
		enc_l += p_enc_l;
		
		USB_PRINTF("enc_data_r,%d\n\r", enc_r);
		USB_PRINTF("p_enc_data_r,%d\n\r", p_enc_r);
		USB_PRINTF("TPU1.TCNT,%d\n\r\n\r", TPU1.TCNT);
		
		USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		USB_PRINTF("p_enc_data_l,%d\n\r", p_enc_l);
		USB_PRINTF("MTU2.TCNT,%d\n\r", MTU2.TCNT);
		//USB_PRINTF("TCFU,%d\n\r",TPU1.TSR.BIT.TCFU);
		//USB_PRINTF("data_M,%d\n\r");
		//USB_PRINTF("data_L,%d\n\r\n\r");
		
		//USB_PRINTF("Z_gyro_data,%d\n\r\bn\r");
		

		//画面クリアシーケンス
		//SCI_printf("\x1b[2J");				//クリアスクリーン[CLS]
		//SCI_printf("\x1b[0;0H");			//カーソルを0,0に移動
		
		USB_PRINTF("\x1b[2J");
		USB_PRINTF("\x1b[0;0H");
		
	}

	
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
