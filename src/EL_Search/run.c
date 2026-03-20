/***********************************************************************/
/*                                                                     */
/*  FILE        :spi.c			                               */
/*  DATE        :Tue, Jun 08, 2017                                     */
/*  DESCRIPTION :SPI Program                                           */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#include "init.h"
#include "spi.h"
#include "parameters.h"
#include "glob_var.h"
#include "mytypedef.h"
#include "portdef.h"
#include "interface.h"

extern wait_ms(int wtime);
extern void USB_PRINTF(char *format, ...);

void straight(float len, float acc, float max_sp, float end_sp){
	I_tar_ang_vel = 0;
	I_ang_vel = 0;
	I_tar_speed = 0;
	I_speed = 0;
	//走行モードを直線にする
	run_mode = STRAIGHT_MODE;
	//壁制御を有効にする
	con_wall.enable = true;
	//目標距離をグローバル変数に代入する
	len_target = len;
	//目標速度を設定
	end_speed = end_sp;
	//加速度を設定
	accel = acc;
	//最高速度を設定
	max_speed = max_sp;
	
	//モータ出力をON
	MOT_POWER_ON;	
	
	if(end_speed == 0){	//最終的に停止する場合
		//減速処理を始めるべき位置まで加速、定速区間を続行
		while( ((len_target -10) - len_mouse) >  1000.0*((float)(tar_speed * tar_speed) - (float)(end_speed * end_speed))/(float)(2.0*accel));/*{
			USB_PRINTF("len_target : %d\n\r",len_target);
			USB_PRINTF("len_mouse  : %d\n\r",len_mouse);
			USB_PRINTF("tar_speed  : %f\n\r",tar_speed);
			USB_PRINTF("\x1b[2J");				//クリアスクリーン[CLS]
			USB_PRINTF("\x1b[0;0H");			//カーソルを0,0に移動
		}*/
		//減速処理開始
		accel = -acc;					//減速するために加速度を負の値にする	
		while(len_mouse < len_target -1){		//停止したい距離の少し手前まで継続
			//一定速度まで減速したら最低駆動トルクで走行
			if(tar_speed <= MIN_SPEED){	//目標速度が最低速度になったら、加速度を0にする
				accel = 0;
				tar_speed = MIN_SPEED;
			}
		}
		accel = 0;
		tar_speed = 0;
		//速度が0以下になるまで逆転する
		while(speed >= 0.0);
			
	}else{
		//減速処理を始めるべき位置まで加速、定速区間を続行
		while( ((len_target-10) - len_mouse) >  1000.0*((float)(tar_speed * tar_speed) - (float)(end_speed * end_speed))/(float)(2.0*accel));
		
		//減速処理開始
		accel = -acc;					//減速するために加速度を負の値にする	
		while(len_mouse < len_target -1){//(len_mouse < (len_target+5)){	//停止したい距離の少し手前まで継続
		/*	if((sen_fr.is_wall==false) && (sen_fl.is_wall==false) && (len_mouse > len_target)){ //前壁無し
				break;
			}
			if( (sen_fr.value > RIGHT_90) && (sen_fl.value > LEFT_90) && ( len_mouse > (len_target -5)) ) {//目標の位置の5mm手前
        			break;
    			}前壁合わせを封印 2021/6/16
		*/	
			//一定速度まで減速したら最低駆動トルクで走行
			if(tar_speed <= end_speed){	//目標速度が最低速度になったら、加速度を0にする
				accel = 0;
				//tar_speed = end_speed;
			}
		}
	}
	//加速度を0にする
	accel = 0;
	//現在距離を0にリセット
	len_mouse = 0;
}

void turn(int deg, float ang_accel, float max_ang_velocity, short dir){
	wait_ms(WAIT_TIME);
	I_tar_ang_vel = 0;
	I_ang_vel = 0;
	I_tar_speed = 0;
	I_speed = 0;
	tar_degree = 0;

	float	local_degree = 0;
	accel = 0;
	tar_speed = 0;
	tar_ang_vel = 0;
	//走行モードをスラロームモードにする
	run_mode = TURN_MODE;

	//回転方向定義
	TURN_DIR = dir;	
	
	//車体の現在角度を取得
	local_degree = degree;
	tar_degree = 0;
	
	//角加速度、加速度、最高角速度設定
	MOT_POWER_ON;
	if(dir == LEFT){
		ang_acc = ang_accel;			//角加速度を設定
		max_ang_vel = max_ang_velocity;
		max_degree = deg;
		while( (max_degree - (degree - local_degree))*PI/180.0 > (tar_ang_vel*tar_ang_vel/(2.0 * ang_acc)));
		
	}else if(dir == RIGHT){
		ang_acc = -ang_accel;			//角加速度を設定
		max_ang_vel = -max_ang_velocity;
		max_degree = -deg;
		while(-(float)(max_degree - (degree - local_degree))*PI/180.0 > (float)(tar_ang_vel*tar_ang_vel/(float)(2.0 * -ang_acc)));
	}

	//BEEP();
	//角減速区間に入るため、角加速度設定
	MOT_POWER_ON;
	if(dir == LEFT){
		ang_acc = -ang_accel;			//角加速度を設定
		//減速区間走行
		while((degree - local_degree) < max_degree){
			if(tar_ang_vel < TURN_MIN_SPEED){
				ang_acc = 0;
				tar_ang_vel = TURN_MIN_SPEED;
			}
		}
		
		ang_acc = 0;
		tar_ang_vel = 0;
		tar_degree = max_degree;
		
	}else if(dir == RIGHT){
		ang_acc = +ang_accel;			//角加速度を設定
		//減速区間走行
		while((degree - local_degree) > max_degree){
			if(-tar_ang_vel < TURN_MIN_SPEED){
				ang_acc = 0;
				tar_ang_vel = -TURN_MIN_SPEED;
			}
		}
		ang_acc = 0;
		tar_ang_vel = 0;
		tar_degree = max_degree;


	}
	
	while(ang_vel >= 0.05 || ang_vel <= -0.05 );
	
	tar_ang_vel = 0;
	ang_acc = 0;
	//現在距離を0にリセット
	len_mouse = 0;
	wait_ms(WAIT_TIME);
}
void back(float len){   //作った。straightのコピー
	unsigned int start_timer;	//タイヤがロックしたときの対策
	I_tar_ang_vel = 0;
	I_ang_vel = 0;
	I_tar_speed = 0;
	//I_speed = 0;
	len_mouse = 0;			//距離の初期化
	//走行モードを直線にする
	run_mode = STRAIGHT_MODE;
	//壁制御を有効にする
	con_wall.enable = false;
	//目標距離をグローバル変数に代入する
	len_target = len;		//ここにはマイナスの値が入る
	//目標速度を設定
	end_speed = 0;
	//加速度を設定
	accel = -1.0;
	//最高速度を設定
	max_speed = 0.1;
	
	//モータ出力をON
	MOT_POWER_ON;	
	
	//減速処理を始めるべき位置まで加速、定速区間を続行
	while( ((len_target -10) - len_mouse) >  1000.0*((float)(tar_speed * tar_speed) - (float)(end_speed * end_speed))/(float)(2.0*accel));
	//減速処理開始
	accel = 1.0;					//減速するために加速度を負の値にする	
	start_timer = timer;
	while(len_mouse > len_target +1){		//停止したい距離の少し手前まで継続
	//一定速度まで減速したら最低駆動トルクで走行
		if(tar_speed >= -1*MIN_SPEED){	//目標速度が最低速度になったら、加速度を0にする
			accel = 0;
			tar_speed = -1*MIN_SPEED;
		}
		if((timer - start_timer) > 1000){
			break;
		}
	}
	accel = 0;
	tar_speed = 0;
	len_mouse = 0;
	//速度が0以下になるまで逆転する
	//while(speed >= 0.0);
}

void stop(void)
{
	I_tar_ang_vel = 0;
	I_ang_vel = 0;
	I_tar_speed = 0;
	
	run_mode = NON_CON_MODE;
	MOT_POWER_OFF;
	
	len_mouse = 0;			//距離の初期化
	accel = 0;
	tar_speed = 0;
	
	wait_ms(5);

}
