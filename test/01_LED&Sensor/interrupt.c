#include "iodefine.h"
#include "portdef.h"
#include "static_parameters.h"
#include "macro.h"
#include "parameters.h"
#include "interrupt.h"

unsigned short getBatteryVolt(void)
{
	S12AD.ADANS0.BIT.ANS0=0x0100;			//AN008
	S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
	while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
	return S12AD.ADDR8;
}

void battery_save(unsigned short v_value)
{
	static int bled_cnt = 0;	//バッテリ電圧監視用LEDのPWM用変数
	
	//バッテリ電圧表示
	bled_cnt++;


	if(bled_cnt > 10){
		bled_cnt= 0;
	}
	
	if( (10.0 * (v_value - BATT_MIN) / (BATT_MAX - BATT_MIN)) > bled_cnt ){
		//LED0 = 1;
		LED0 = 0;
	}else{
		//LED0 = 0;
		LED0 = 1;
	}
	
	if(v_value < BATT_MIN){
		SET_BUZZER_FREQ(400);	//値が増加した時の周波数を設定				
		ENABLE_BUZZER;		//ブザーを発振する
		MOT_POWER_OFF;		//モータを停止する
		//LED0 = LED1 = LED2 = LED3 = SLED_L = SLED_FL = SLED_FR = SLED_R = 0;	//LEDを停止する
		LED0 = LED1 = LED2 = LED3 = LED4 = S_LED = 0;	//LEDを停止する
	}
}
void int_cmt0(void)
{
	battery_save(getBatteryVolt());	//バッテリ監視
}		

void int_cmt1(void)		//センサ読み込み用り込み
{
	static int state = 0;	//読み込むセンサのローテーション管理用変数
	int i;
	
	switch(state)
	{
		case 0:		//前右センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			
			sen_fl.p_value = sen_fl.value;			//過去の値を保存
			sen_fl.value = S12AD.ADDR6;			//値を保存

			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_r.p_value = sen_r.value;			//過去の値を保存
			sen_fl.value = S12AD.ADDR6 - sen_fl.value;	//値を保存
			
			break;
			
			/*S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			sen_r.p_value = sen_r.value;			//過去の値を保存
			sen_r.value = S12AD.ADDR6;			//値を保存
			
			break;*/
			
			

		case 1:		//左センサ読み込み

			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0010;			//AN004
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_r.p_value = sen_r.value;			//過去の値を保存
			sen_r.value = S12AD.ADDR4;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0010;			//AN004
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_fl.p_value = sen_fl.value;			//過去の値を保存
			sen_r.value = S12AD.ADDR4 - sen_r.value;		//値を保存
			
			break;
			
			/*S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0010;			//AN004
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			sen_fl.p_value = sen_fl.value;			//過去の値を保存
			sen_fl.value = S12AD.ADDR4;			//値を保存
			
			break;*/

		case 2:		//右センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_l.p_value = sen_l.value;			//過去の値を保存
			sen_l.value = S12AD.ADDR9;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_fr.p_value = sen_fr.value;			//過去の値を保存
			sen_l.value = S12AD.ADDR9 - sen_l.value;			//値を保存
		
			break;
			
			/*S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			sen_fr.p_value = sen_fr.value;			//過去の値を保存
			sen_fr.value = S12AD.ADDR9;			//値を保存
		
			break;*/


		case 3:		//前左センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_fr.p_value = sen_fr.value;			//過去の値を保存
			sen_fr.value = S12AD.ADDR2;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_l.p_value = sen_l.value;			//過去の値を保存
			sen_fr.value = S12AD.ADDR2 - sen_fr.value;	//値を保存

			break;
			
			/*S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			sen_l.p_value = sen_l.value;			//過去の値を保存
			sen_l.value = S12AD.ADDR2;			//値を保存

			break;*/
	}
	
	state++;							//４回ごとに繰り返す
	if(state > 3)
	{
		state = 0;
	}
		
}