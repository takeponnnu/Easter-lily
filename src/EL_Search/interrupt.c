#include "iodefine.h"
#include "portdef.h"
#include "static_parameters.h"
#include "macro.h"
#include "parameters.h"
#include "interrupt.h"
#include "spi.h"
#include "interface.h"
#include "glob_var.h"
#include "run.h"
#include "mytypedef.h"
#include "machine.h"	//これを入れておかないとnop();がエラーになる。stdio.h みたいな標準ファイルらしい?

extern int get_encoder_r(void);
extern int get_encoder_l(void);

/*unsigned short getBatteryVolt(void)
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
}*/
void int_cmt0(void)
{
	/*****************************************************************************************
	目標速度生成
		tar_speedの生成
		など
	*****************************************************************************************/
	//直線の場合の目標速度生成
	if(run_mode == STRAIGHT_MODE){
		tar_speed += accel/1000.0;	//目標速度を設定加速度で更新
		//最高速度制限
		if(tar_speed > max_speed){
			tar_speed = max_speed;	//目標速度を設定最高速度に設定
		}
				
	
	}else if(run_mode == TURN_MODE){
		
		//車体中心速度更新
		tar_speed += accel/1000;
		//最高速度制限
		if(tar_speed > max_speed){
			tar_speed = max_speed;	//目標速度を設定最高速度に設定
		}
		
		//角加速度更新
		tar_ang_vel += ang_acc/1000.0;	//目標角速度を設定加速度で更新
		tar_degree  += (tar_ang_vel*180.0/PI)/1000.0;

		
		//左回転の場合
		if(TURN_DIR == LEFT){
			//最高角速度制限
			if(tar_ang_vel > max_ang_vel){
				tar_ang_vel = max_ang_vel;	//目標速度を設定最高速度に設定
			}
			if(tar_degree > max_degree){
				tar_degree = max_degree;
			}
		}else if(TURN_DIR == RIGHT){
		//右回転の場合
			//最高角速度制限
			if(tar_ang_vel < max_ang_vel){
				tar_ang_vel = max_ang_vel;	//目標速度を設定最高速度に設定
			}
			if(tar_degree < max_degree){
				tar_degree = max_degree;
			}
		}
		
	}else if(run_mode == NON_CON_MODE){
		//何もしない
		nop();	
	}else{
		//何もしない
		nop();	
	}
	
	/*****************************************************************************************
	壁制御
		横壁センサによる目標角度生成
	*****************************************************************************************/
	if(run_mode == STRAIGHT_MODE){
		if(con_wall.enable == true && sen_fr.value + sen_fl.value <= (TH_SEN_FR+TH_SEN_FL)*5 && (( sen_r.is_control == true ) || ( sen_l.is_control == true )) )		//壁制御が許可されているかチェック
		{
			
			con_wall.p_error = con_wall.error;	//過去の偏差を保存
			
			
			//左右のセンサが、それぞれ使える状態であるかどうかチェックして、姿勢制御の偏差を計算
			if( ( sen_r.is_control == true ) && ( sen_l.is_control == true ) )
			{									//両方とも有効だった場合の偏差を計算
				con_wall.error = sen_r.error - sen_l.error;
			}
			else								//片方もしくは両方のセンサが無効だった場合の偏差を計算
			{
				con_wall.error = /*2.0 **/ (sen_r.error - sen_l.error);	//片方しか使用しないので2倍する のをやめた
			}
		
			
			//DI制御計算
			con_wall.diff = con_wall.error - con_wall.p_error;	//偏差の微分値を計算
			con_wall.sum += con_wall.error;				//偏差の積分値を計算
			
			if(con_wall.sum > con_wall.sum_max)			//偏差の積分値の最大値を制限
			{
				con_wall.sum = con_wall.sum_max;
			}
			else if(con_wall.sum < (-con_wall.sum_max))		//偏差の積分値の最低値を制限
			{
				con_wall.sum = -con_wall.sum_max;
			}

			con_wall.p_omega = con_wall.omega;
			con_wall.omega = con_wall.kp * con_wall.error * 0.5 + con_wall.p_omega * 0.5;	//現在の目標角速度[rad/s]を計算
			tar_ang_vel = con_wall.omega;
		}else{
			tar_ang_vel = 0;
		}
		
	}else if(run_mode == NON_CON_MODE){	
			//何もしない
			nop();
	}else{
		nop();	
	}
	
	/*****************************************************************************************
	壁制御かスラロームの理想値によって生成された
	目標速度と目標角角度の積分
	*****************************************************************************************/
	I_tar_speed += tar_speed;
	if(I_tar_speed >30*10000000000){
		I_tar_speed = 30*10000000000;
	}else if(I_tar_speed < -1*10000000000){
		I_tar_speed = 1*10000000000;
	}
	
	I_tar_ang_vel += tar_ang_vel;
	if(I_tar_ang_vel >30*10000000000){
		I_tar_ang_vel = 30*10000000000;
	}else if(I_tar_ang_vel < -1*10000000000){
		I_tar_ang_vel = 1*10000000000;
	}
	/*****************************************************************************************
	目標速度の偏差から出力電圧にフィードバック
		
	*****************************************************************************************/
	//フィードバック制御
	V_r = V_l = 0.0;
	if(run_mode == STRAIGHT_MODE || run_mode == TURN_MODE){
	//直進時のフィードバック制御
		//左右モータのフィードバック
		//速度に対するP制御
		V_r += 1 * (tar_speed - speed) *SPEED_KP/1.0; //15目標値付近で発振
		V_l += 1 * (tar_speed - speed) *SPEED_KP/1.0;
		//速度に対するI制御
		V_r += 1 * (I_tar_speed - I_speed) *SPEED_KI/1.0; //(0.4-0.3)*0.1 -> 0.01 
		V_l += 1 * (I_tar_speed - I_speed) *SPEED_KI/1.0;
		//速度に対するD制御
		V_r -= 1 * (p_speed - speed) *SPEED_KD/1.0; //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (p_speed - speed) *SPEED_KD/1.0;

		//角速度に対するP制御
		V_r += 1 * (tar_ang_vel - ang_vel) *(OMEGA_KP/100.0);
		V_l -= 1 * (tar_ang_vel - ang_vel) *(OMEGA_KP/100.0);
		//角速度に対するI制御
		V_r += 1 * (I_tar_ang_vel - I_ang_vel) *(OMEGA_KI/100.0); //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (I_tar_ang_vel - I_ang_vel) *(OMEGA_KI/100.0);
		//角速度に対するD制御
		V_r += 1 * (p_ang_vel - ang_vel) *(OMEGA_KD/100.0); //(0.4-0.3)*0.1 -> 0.01 
		V_l -= 1 * (p_ang_vel - ang_vel) *(OMEGA_KD/100.0);

	}else if(run_mode == NON_CON_MODE){
		//何もしない
		nop();
	}else{
		//何もしない
		nop();	
	}
	
	/*****************************************************************************************
	出力電圧からモータの回転方向を決定
		電圧は絶対値を入力するため、
		正の値の時はモータを正転設定にして、電圧はそのまま
		負の値の時はモータを逆転設定にして、電圧を反転する
	*****************************************************************************************/
	if(run_mode != TEST_MODE){
		//右モータの出力電圧が正の場合
		if(V_r > 0){
			//モータを正転に設定
			MOT_CWCCW_R1 = MOT_R1_FORWARD;	//右モータを正転に設定
			MOT_CWCCW_R2 = MOT_R2_FORWARD;
			//V_r = V_r;			//電圧は正なのでそのまま
		}else{
		//右モータの出力電圧が負の場合
			MOT_CWCCW_R1  = MOT_R1_BACK;	//右モータを逆回転に設定
			MOT_CWCCW_R2  = MOT_R2_BACK;
			V_r = -V_r;			//電圧を正の値へ反転
		}
		
		//左モータの出力電圧が正の場合
		if(V_l > 0){
			//モータを正転に設定
			MOT_CWCCW_L1 = MOT_L1_FORWARD;	//左モータを正転に設定
			MOT_CWCCW_L2 = MOT_L2_FORWARD;
			//V_l = V_l;			//電圧は正なのでそのまま
		}else{
		//左モータの出力電圧が負の場合
			MOT_CWCCW_L1  = MOT_L1_BACK;	//左モータを逆回転に設定
			MOT_CWCCW_L2  = MOT_L2_BACK;
			V_l = -V_l;			//電圧を正の値へ反転
		}
	}	
	/*****************************************************************************************
	出力電圧の制限
		モータへの出力電圧の上限を2Vに制限
	*****************************************************************************************/
	//右モータに印加する電圧が2.0Vを超えたら強制的に2.0Vに変更
	if(V_r > 5.0){
		V_r = 5.0;
	}
	//左モータに印加する電圧が2.0Vを超えたら強制的に2.0Vに変更
	if(V_l > 5.0){
		V_l = 5.0;
	}
	
	/*****************************************************************************************
	モータへ出力
		
	*****************************************************************************************/
	
	//バッテリー電圧からデューティを計算
	Duty_r = V_r/V_bat;
	Duty_l = V_l/V_bat;
	
	//モータにPWMを出力
	if(run_mode != TEST_MODE){
		MOT_OUT_R =(short)(480.0 * Duty_r);	// MTUの周期を入れる。HM 240.0(200kHz)
		MOT_OUT_L =(short)(480.0 * Duty_l);	// 480(100kHz)
	}
	
	/*****************************************************************************************
	タイマのカウント
		
	*****************************************************************************************/
	timer++;
	cnt++;
	
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
			
			if(sen_fr.value > sen_fr.th_wall)		//壁の有無を判断
			{
				sen_fr.is_wall = true;			//右前壁あり
			}
			else
			{
				sen_fr.is_wall = false;			//右前壁なし
			}

			break;

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

			if(sen_l.value > sen_l.th_wall)			//壁の有無を判断
			{
				sen_l.is_wall = true;			//左壁あり
			}
			else
			{
				sen_l.is_wall = false;			//左壁なし
			}
			
			if(sen_l.value > sen_l.th_control)		//制御をかけるか否かを判断
			{
				sen_l.error = sen_l.value - sen_l.ref;	//制御をかける場合は偏差を計算する
				sen_l.is_control = true;		//左センサを制御に使う
			}
			else
			{
				sen_l.error = 0;			//制御に使わない場合は偏差を0にしておく
				sen_l.is_control = false;		//左センサを制御に使わない
			}

			break;

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

			if(sen_r.value > sen_r.th_wall)			//壁の有無を判断
			{
				sen_r.is_wall = true;			//右壁あり
			}
			else
			{
				sen_r.is_wall = false;			//右壁なし
			}
			
			if(sen_r.value > sen_r.th_control)		//制御をかけるか否かを判断
			{
				sen_r.error = sen_r.value - sen_r.ref;	//制御をかける場合は偏差を計算
				sen_r.is_control = true;		//右センサを制御に使う
			}
			else
			{
				sen_r.error = 0;			//制御に使わない場合は偏差を0にしておく
				sen_r.is_control = false;		//右センサを制御に使わない
			}
		
			break;

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
			
			if(sen_fl.value > sen_fl.th_wall)		//壁の有無を判断
			{
				sen_fl.is_wall = true;			//左前壁あり
			}
			else
			{
				sen_fl.is_wall = false;			//左前壁なし
			}

			break;

	}
	
	state++;							//４回ごとに繰り返す
	if(state > 3)
	{
		state = 0;
	}
	
	S12AD.ADANS0.BIT.ANS0=0x0100;			//AN008
	S12AD.ADCSR.BIT.ADST=1;				//AD変換開始
	while(S12AD.ADCSR.BIT.ADST);			//AD変換終了まで待つ
	V_bat = (4.3*3.3*(float)(S12AD.ADDR8/4095.0) );	//4.3は分圧の係数、3.3は多分マイコンの電源電圧、4095は分解能?
	if(V_bat < 8.0){
		LED0 = 1;
	}
	if(V_bat < 7.8){
		
		//モータ止める
		Duty_r = 0;
		Duty_l = 0;
		MOT_POWER_OFF;	//PC6(SLEEPピン)
		
		//ブザー鳴らし続ける
		while(1){
			BEEP();
			tone (NOTE_C5 , 1000);
		}
	}
	/*****************************************************************************************
	1kHzごとにログを取得
		
	*****************************************************************************************/
	
	if(log_timer % 4 == 0 && log_flag==1 ){
		if(log_timer < (LOG_CNT*4)){

			log[0][log_timer/4] = (int)(len_mouse);
			log[1][log_timer/4] = (int)(1000*tar_speed);
			log[2][log_timer/4] = (int)(1000*speed);
			log[3][log_timer/4] = (int)(100*Duty_r);
			log[4][log_timer/4] = (int)(100*Duty_l);
			log[5][log_timer/4] = (int)(1000*V_bat);
			log[6][log_timer/4] = (int)(tar_degree*10);
			log[7][log_timer/4] = (int)(degree*10);
			log[8][log_timer/4] = (int)(tar_ang_vel*1000);
			log[9][log_timer/4] = (int)(ang_vel*1000);
			log[10][log_timer/4] = (int)(I_tar_ang_vel);
			log[11][log_timer/4] = (int)(ang_acc*1000);
			log[12][log_timer/4] = locate_r;
			log[13][log_timer/4] = locate_l;
		}
	}	
	
	log_timer++;
		
}

void int_cmt2(void)
{
 	static unsigned int	enc_data_r;	//エンコーダの生データ
	static unsigned int	enc_data_l;	//エンコーダの生データ 
	//static int enc_data_r;
	//static int enc_data_l;
	//int p_enc_r;
	//int p_enc_l;
	static short	state;
	/*****************************************************************************************
	エンコーダ関連
		値の取得　速度更新　距離積分など
	*****************************************************************************************/	
   if(state == 0){
		//RSPI0.SPCMD0.BIT.SSLA = 	0x00;	//SSL信号アサート設定(SSL0を使う)
		//preprocess_spi_enc(0xFFFF);	//Read Angle
		
		//	エンコーダのデータの出力の仕方が違う事に気付いたので変更 2021/6/1
		enc_r += get_encoder_r();	// locate_rの値がHMと同じにするために追加。
		if(enc_r>=2048)	enc_r = enc_r - 2048;
		if(enc_r< 0 )	enc_r = enc_r + 2048;
		enc_data_r = enc_r;
		
		//enc_r += p_enc_r;
		//enc_data_r = get_encoder_r();
		state = 1;
	}else{
		//RSPI0.SPCMD0.BIT.SSLA = 	0x02;	//SSL信号アサート設定(SSL2を使う)
		//preprocess_spi_enc(0xFFFF);	//Read Angle
		
		//	Rと同じ
		enc_l += get_encoder_l();	// locate_/の値がHMと同じにするために追加。
		if(enc_l>=2048)	enc_l = enc_l - 2048;
		if(enc_l< 0 )	enc_l = enc_l + 2048;
		enc_data_l = enc_l;
		
		//enc_l += p_enc_l;
		//enc_data_l = get_encoder_l();
		
		//左右エンコーダから角度取得
		//4096で一回転(360deg = 0deg)
		locate_r =  enc_data_r;		// 前進したらマイナス方向に増えていくはずだけど、ここには整数しか入ってないかも
		locate_l =  enc_data_l;
		
		//右エンコーダの現在の位置と,1msec前の位置との差分を計算
		//単位時間（1msec）あたりの変位量を計算
		diff_pulse_r = (locate_r - before_locate_r);
		
		//	2021/6/1
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
		//diff_pulse_l = (-locate_l + before_locate_l);
		diff_pulse_l = (locate_l - before_locate_l);
		
		//
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
		len_mouse_int = len_mouse;
		
		//過去の値を保存
		before_locate_r = locate_r;
		before_locate_l = locate_l;
				
		state = 0;
	}
	
	
	/*****************************************************************************************
	ジャイロ関連(ヨー軸)
		値の取得　角度の積分　
	*****************************************************************************************/
	if(state == 0){
		//ジャイロセンサの値の更新
		preprocess_spi_gyro(0xA90000);		//b70000
		
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

