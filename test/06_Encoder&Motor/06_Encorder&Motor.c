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
extern void wait_ms(int wtime);	

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
	
	USB_PRINTF("START");
	wait_ms(1000);
	
	//spiのイニシャライズ
	//init_spi_gyro();
	
	/*while(1){
		USB_PRINTF("Cheak \n\r");
		USB_PRINTF("OK!! \n\r");
		
		USB_PRINTF("\x1b[2J");
		USB_PRINTF("\x1b[0;0H");
	}*/
	
	int p_enc_r = 0;
	int p_enc_l = 0;
	
	MTU.TSTR.BIT.CST2 = 1;
	MTU.TSTR.BIT.CST3 = 1;
	MTU.TSTR.BIT.CST4 = 1;	//カウントスタート
	TPUA.TSTR.BIT.CST1 = 1;
	
	//MOT_POWER_ON;
	PORTB.PODR.BIT.B1 = 1;
	
	for(int i=0;i<3;i++){
		//MOT_POWER_ON;
		//PORTB.PODR.BIT.B1 = 1;
		PORTA.PODR.BIT.B0 = 0;	//R_PH BIN1	後退の指令（前進[エンコーダ＋方向]）
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 0;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		enc_r = enc_l = 0;
		MTU4.TGRC = (96-1);				//duty 0.2
		MTU3.TGRD = (96-1);
		
	/*	MTU.TSTR.BIT.CST2 = 1;
		MTU.TSTR.BIT.CST3 = 1;
		MTU.TSTR.BIT.CST4 = 1;	//カウントスタート
		TPUA.TSTR.BIT.CST1 = 1;	
	*/	
		while( enc_r < 200000 || enc_l < 200000 ){
			p_enc_r = get_encoder_r();
			enc_r += p_enc_r;
		
			p_enc_l = get_encoder_l();
			enc_l += p_enc_l;
			
			USB_PRINTF("enc_data_r,%d\n\r", enc_r);
			USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		
			USB_PRINTF("\x1b[2J");
			USB_PRINTF("\x1b[0;0H");
		}
		
		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		wait_ms(500);
		
		
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 0;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 0;	//L_PH AIN2
		
		enc_r = enc_l = 0;
		MTU4.TGRC = (192-1);			//duty 0.4
		MTU3.TGRD = (192-1);
		
		while( enc_r > -200000 || enc_l > -200000 ){
			p_enc_r = get_encoder_r();
			enc_r += p_enc_r;
		
			p_enc_l = get_encoder_l();
			enc_l += p_enc_l;
			
			USB_PRINTF("enc_data_r,%d\n\r", enc_r);
			USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		
			USB_PRINTF("\x1b[2J");
			USB_PRINTF("\x1b[0;0H");
		}
		
		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		wait_ms(500);
		
		PORTA.PODR.BIT.B0 = 0;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 0;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		enc_r = enc_l = 0;
		MTU4.TGRC = (288-1);			//duty 0.6
		MTU3.TGRD = (288-1);
		
		while( enc_r < 200000 || enc_l < 200000 ){
			p_enc_r = get_encoder_r();
			enc_r += p_enc_r;
		
			p_enc_l = get_encoder_l();
			enc_l += p_enc_l;
			
			USB_PRINTF("enc_data_r,%d\n\r", enc_r);
			USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		
			USB_PRINTF("\x1b[2J");
			USB_PRINTF("\x1b[0;0H");
		}
		
		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		wait_ms(500);
		
		
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 0;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 0;	//L_PH AIN2
		
		enc_r = enc_l = 0;
		MTU4.TGRC = (384-1);			//duty 0.8
		MTU3.TGRD = (384-1);
		
		while( enc_r > -200000 || enc_l > -200000 ){
			p_enc_r = get_encoder_r();
			enc_r += p_enc_r;
		
			p_enc_l = get_encoder_l();
			enc_l += p_enc_l;
			
			USB_PRINTF("enc_data_r,%d\n\r", enc_r);
			USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		
			USB_PRINTF("\x1b[2J");
			USB_PRINTF("\x1b[0;0H");
		}
		
		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		wait_ms(500);
		
		PORTA.PODR.BIT.B0 = 0;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 0;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		enc_r = enc_l = 0;
		MTU4.TGRC = (432-1);			//duty 0.9
		MTU3.TGRD = (432-1);
		
		while( enc_r < 200000 || enc_l < 200000 ){
			p_enc_r = get_encoder_r();
			enc_r += p_enc_r;
		
			p_enc_l = get_encoder_l();
			enc_l += p_enc_l;
			
			USB_PRINTF("enc_data_r,%d\n\r", enc_r);
			USB_PRINTF("enc_data_l,%d\n\r", enc_l);
		
			USB_PRINTF("\x1b[2J");
			USB_PRINTF("\x1b[0;0H");
		}
		
		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2
		
		wait_ms(500);
		
	}

		//ショートブレーキ
		PORTA.PODR.BIT.B0 = 1;	//R_PH BIN1
		PORTA.PODR.BIT.B4 = 1;	//R_PH BIN2
		PORTB.PODR.BIT.B0 = 1;	//L_PH AIN1
		PORTA.PODR.BIT.B6 = 1;	//L_PH AIN2

	
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
