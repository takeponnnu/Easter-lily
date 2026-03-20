#include "iodefine.h"
#include "sci.h"
#include "mytypedef.h"
#include "portdef.h"
#include "parameters.h"

extern t_sensor sen_r,sen_l,sen_fr,sen_fl;	//外部宣言のセンサ構造体
extern void USB_PRINTF(char *format, ...);

void view_adc(void)		//センサーの値を見る
{
	unsigned int i;

	//MOT_POWER_ON;		// ステッピングモータを励磁する

	while(1)
	{
		USB_PRINTF("r_sen  is\t%d\r\n",(sen_r.value));	//右センサの値
		USB_PRINTF("fr_sen is\t%d\r\n",(sen_fr.value));	//右前センサの値
		USB_PRINTF("fl_sen is\t%d\r\n",(sen_fl.value));	//左前センサの値
		USB_PRINTF("l_sen  is\t%d\r\n",(sen_l.value));	//左センサの値
		USB_PRINTF("VDD    is\t%dmV\r\n",((10+51)/10*3300*S12AD.ADDR8/4095));//電圧を表示

		for(i = 0; i < 0x3fffff; i++);					//表示間隔を設定

		//画面クリアシーケンス
		USB_PRINTF("\x1b[2J");		//クリアスクリーン[CLS]
		USB_PRINTF("\x1b[0;0H");	//カーソルを0,0に移動
	}
}
