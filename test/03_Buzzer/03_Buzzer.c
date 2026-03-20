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
	I/Oピンの設定
		ブザー用IOポートの設定
	*****************************************************************************************/
	//ブザー関連
	PORTB.PDR.BIT.B3 = 1;
	
	PORTA.PDR.BIT.B3 = 1;
	PORTC.PDR.BIT.B3 = 1;
	PORTB.PDR.BIT.B7 = 1;
	PORT5.PDR.BIT.B5 = 1;
	PORT5.PDR.BIT.B4 = 1;
	
	PORTA.PODR.BIT.B3 = 1;
	PORTC.PODR.BIT.B3 = 1;
	PORTB.PODR.BIT.B7 = 1;
	PORT5.PODR.BIT.B5 = 1;
	PORT5.PODR.BIT.B4 = 1;
	/*****************************************************************************************
	メインプログラム
		ブザーON
	*****************************************************************************************/
	int i = 0;
	while(1){
		PORTB.PODR.BIT.B3 = 1;
		for(i = 0; i < 100; i++);
		PORTB.PODR.BIT.B3 = 0;
		for(i = 0; i < 100; i++);
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
