#include "iodefine.h"
#include "portdef.h"
#include "static_parameters.h"
#include "macro.h"
#include "parameters.h"
#include "interrupt.h"
#include "spi.h"

#include "glob_var.h"
#include "machine.h"

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
		case 0:		//右センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			
			sen_r.p_value = sen_r.value;			//過去の値を保存
			sen_r.value = S12AD.ADDR6;			//値を保存

			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0040;			//AN006
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_r.p_value = sen_r.value;			//過去の値を保存
			sen_r.value = S12AD.ADDR6 - sen_r.value;	//値を保存
			
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
			
			

		case 1:		//前左センサ読み込み

			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0010;			//AN004
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_fl.p_value = sen_fl.value;			//過去の値を保存
			sen_fl.value = S12AD.ADDR4;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0010;			//AN004
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_fl.p_value = sen_fl.value;			//過去の値を保存
			sen_fl.value = S12AD.ADDR4 - sen_fl.value;		//値を保存
			
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

		case 2:		//前右センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_fr.p_value = sen_fr.value;			//過去の値を保存
			sen_fr.value = S12AD.ADDR9;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0200;			//AN009
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_fr.p_value = sen_fr.value;			//過去の値を保存
			sen_fr.value = S12AD.ADDR9 - sen_fr.value;			//値を保存
		
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


		case 3:		//左センサ読み込み
		
			S_LED = 0;
			
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ

			sen_l.p_value = sen_l.value;			//過去の値を保存
			sen_l.value = S12AD.ADDR2;			//値を保存
		
			S_LED = 1;					//LED点灯
			for(i = 0; i < WAITLOOP_SLED; i++)	;	//フォトトランジスタの応答待ちループ
			S12AD.ADANS0.BIT.ANS0=0x0004;			//AN002
			S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
			while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
			S_LED = 0;					//LED消灯

			//sen_l.p_value = sen_l.value;			//過去の値を保存
			sen_l.value = S12AD.ADDR2 - sen_l.value;	//値を保存

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

void int_cmt2(void)
{
 	static unsigned int	enc_data_r;	//エンコーダの生データ
	static unsigned int	enc_data_l;	//エンコーダの生データ 
	static short	state;
	/*****************************************************************************************
	エンコーダ関連
		値の取得　速度更新　距離積分など
	*****************************************************************************************/	
   /*if(state == 0){
		RSPI0.SPCMD0.BIT.SSLA = 	0x00;	//SSL信号アサート設定(SSL0を使う)
		preprocess_spi_enc(0xFFFF);	//Read Angle
		enc_data_r = Get_enc_data();
		state = 1;
	}else{
		RSPI0.SPCMD0.BIT.SSLA = 	0x02;	//SSL信号アサート設定(SSL2を使う)
		preprocess_spi_enc(0xFFFF);	//Read Angle
		enc_data_l = Get_enc_data();
		
		//左右エンコーダから角度取得
		//4096で一回転(360deg = 0deg)
		locate_r = enc_data_r;
		locate_l = enc_data_l;
		
		//右エンコーダの現在の位置と,1msec前の位置との差分を計算
		//単位時間（1msec）あたりの変位量を計算
		diff_pulse_r = (locate_r - before_locate_r);
		//変化点を1023から0//へ移動したときの補正
		if((diff_pulse_r > ENC_RES_HALF || diff_pulse_r < -ENC_RES_HALF) && before_locate_r >ENC_RES_HALF){
			diff_pulse_r = (((ENC_RES_MAX - 1) - before_locate_r) + locate_r);
		}
		//変化点を0から1023へ移動したときの補正
		else if((diff_pulse_r > ENC_RES_HALF || diff_pulse_r < -ENC_RES_HALF) && before_locate_r <=ENC_RES_HALF){
			diff_pulse_r = 1*(before_locate_r + ((ENC_RES_MAX - 1) - locate_r));
		}
		
		//左エンコーダの現在の位置と,1msec前の位置との差分を計算
		//単位時間（1msec）あたりの変位量を計算
		diff_pulse_l = (-locate_l + before_locate_l);
		//変化点を1023から0//へ移動したときの補正
		if((diff_pulse_l > ENC_RES_HALF || diff_pulse_l < -ENC_RES_HALF) && before_locate_l >ENC_RES_HALF){
			diff_pulse_l = 1*(((ENC_RES_MAX - 1) - before_locate_l) + locate_l);
		}
		//変化点を0から1023へ移動したときの補正
		else if((diff_pulse_l > ENC_RES_HALF || diff_pulse_l < -ENC_RES_HALF) && before_locate_l <=ENC_RES_HALF){
			diff_pulse_l = (before_locate_l + ((ENC_RES_MAX - 1) - locate_l));
		}
				
		//現在速度を算出
		speed_new_r = (float)((float)diff_pulse_r * (float)MMPP);
		speed_new_l = (float)((float)diff_pulse_l * (float)MMPP);
		
		//過去の値を保存
		speed_old_r= speed_r;
		speed_old_l= speed_l;
		
		//速度のローパスフィルタ
		speed_r = speed_new_r * 0.1 + speed_old_r * 0.9;
		speed_l = speed_new_l * 0.1 + speed_old_l * 0.9;
		
		p_speed = speed;
		//車体速度を計算
		speed = ((speed_r + speed_l)/2.0);
		
		//I成分のオーバーフローとアンダーフロー対策
		I_speed += speed;
		if(I_speed >30*10000000000){
			I_speed = 30*10000000000;
		}else if(I_speed < -1*10000000000){
			I_speed = -1*10000000000;
		}


		
		//距離の計算
		len_mouse += (speed_new_r + speed_new_l)/2.0;
		
		//過去の値を保存
		before_locate_r = locate_r;
		before_locate_l = locate_l;
				
		state = 0;
	}*/
	
	
	/*****************************************************************************************
	ジャイロ関連(ヨー軸)
		値の取得　角度の積分　
	*****************************************************************************************/
	if(state == 0){
		//ジャイロセンサの値の更新
		preprocess_spi_gyro(0xC70000);		//b70000
		
		//LowPass Filter
		gyro_x_new = (float)((short)(read_gyro_data() & 0x0000FFFF));
		gyro_x = (gyro_x_new -gyro_ref );
		
		//角速度の更新
		p_ang_vel = ang_vel;
		ang_vel = ((2000.0*(gyro_x)/32767.0))*3.1415/180.0;
		//積分値の更新
		I_ang_vel += ang_vel;
		if(I_ang_vel >30*10000000000){
			I_ang_vel = 30*10000000000;
		}else if(I_ang_vel < -1*10000000000){
			I_ang_vel = -1*10000000000;
		}
		
		//ジャイロの値を角度に変換
		degree += (2.0*(gyro_x_new - gyro_ref)/32767.0);
		
	}	
	
}

