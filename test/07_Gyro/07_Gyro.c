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
#include <spi.h>


extern void view_adc(void)	;
extern void USB_PRINTF(char *format, ...);

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
	init_spi_gyro();
	
	unsigned long i = 0;
	long data = 0;

	for(i = 0; i < 100*1000*10; i++);
	preprocess_spi_gyro(0xA90000);		//0x062100
	//preprocess_spi_gyro(0xF50000);
	data = read_gyro_data();
	for(i = 0; i < 100*1000*10; i++);
	
	while(1){
		//SCI_printf("data_H,%d\n\r",(data & 0x00FF00) >> 8);
		//SCI_printf("data_L,%d\n\r\n\r",(data & 0x0000FF));
		
		USB_PRINTF("data_H,%d\n\r",(data & 0xFF0000) >> 16);
		USB_PRINTF("data_M,%d\n\r",(data & 0x00FF00) >> 8);
		USB_PRINTF("data_L,%d\n\r\n\r",(data & 0x0000FF));
		
		USB_PRINTF("Z_gyro_data,%d\n\r\bn\r",data );

		//SCI_printf("Ang_Velocity[deg/s],%d\n\r",((int)((2000.0*((float)data))/32767.0)));
		
		//USB_PRINTF("WHO_AM_I = %d\n\r",(data & 0x00FF00) >> 8);
		
		USB_PRINTF("Ang_Velocity[deg/s],%d\n\r",((int)((2000.0*((float)data))/32767.0)));
		
		for(i = 0; i < 100*1000*10; i++);
		for(i = 0; i < 100*1000*100; i++);
		//画面クリアシーケンス
		//SCI_printf("\x1b[2J");				//クリアスクリーン[CLS]
		//SCI_printf("\x1b[0;0H");			//カーソルを0,0に移動
		
		USB_PRINTF("\x1b[2J");
		USB_PRINTF("\x1b[0;0H");
		
		preprocess_spi_gyro(0xA90000);			//B70000
		//preprocess_spi_gyro(0xF50000);
		//data = read_gyro_data();
		data = (short)(read_gyro_data()&0x0000FFFF);
	}

	
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
