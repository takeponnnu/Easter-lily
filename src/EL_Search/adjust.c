/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :2017/6/27	                                       */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :RX631 48P                                             */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "mathf.h"
#include "init.h"
#include "spi.h"
//#include "i2c.h"
#include "parameters.h"
#include "glob_var.h"
#include "run.h"
#include "interface.h"
#include "DataFlash.h"
#include "portdef.h"
#include "fast.h"
#include "search.h"

extern void wait_ms(int wtime);
extern void USB_PRINTF(char *format, ...);
extern void beep( int x );

void adjust(void)
{

	int i = 0;
	int flash_time = 0;
	char flash;
	short ad_mode = 1;
	LED(0);
	while(1){
		switch(ad_mode){

			
			case 1:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	X	X	*
				*					*
				*****************************************/
				
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					//壁制御を有効にする
					con_wall.enable = true;
					gyro_get_ref();
					while(1){
						//A/D sensor
						USB_PRINTF("sen_r.value: %d\n\r",sen_r.value);
						USB_PRINTF("sen_l.value: %d\n\r",sen_l.value);
						USB_PRINTF("sen_fr.value: %d\n\r",sen_fr.value);
						USB_PRINTF("sen_fl.value: %d\n\r",sen_fl.value);
						USB_PRINTF("V_bat: %d\n\r",(int)(V_bat*1000));
						USB_PRINTF("sen_r.th_wall: %d\n\r",sen_r.th_wall);
						USB_PRINTF("sen_l.th_wall: %d\n\r",sen_l.th_wall);
						USB_PRINTF("sen_fr.th_wall: %d\n\r",sen_fr.th_wall);
						USB_PRINTF("sen_fl.th_wall: %d\n\r",sen_fl.th_wall);
						USB_PRINTF("con_wall.omega: %d\n\r",(int)(con_wall.omega*1000));
						USB_PRINTF("speed_r: %d\n\r", (int)(speed_r*100));
						USB_PRINTF("speed_l: %d\n\r", (int)(speed_l*100));
						USB_PRINTF("speed: %d\n\r", (int)(speed*100));
						//gyro
						USB_PRINTF("degree: %d\n\r",(int)degree*10);;			
						USB_PRINTF("gyro: %d\n\r", (int)(ang_vel*1000) );
						//encoder
						USB_PRINTF("locate_r: %d\n\r",locate_r);
						USB_PRINTF("locate_l: %d\n\r",locate_l);	
						USB_PRINTF("len_mouse  : %d\n\r",len_mouse_int);
						USB_PRINTF("diff_pulse_r: %d\n\r",diff_pulse_r);
						USB_PRINTF("diff_pulse_l: %d\n\r",diff_pulse_l);
						USB_PRINTF("enc_r: %d\n\r",enc_r);
						USB_PRINTF("enc_l: %d\n\r",enc_l);
					
						//switch
						USB_PRINTF("switchC: %d\n\r",SW_C);
						USB_PRINTF("switchR: %d\n\r",SW_R);
						USB_PRINTF("switchL: %d\n\r",SW_L);
						wait_ms(100);
						//画面クリアシーケンス
						USB_PRINTF("\x1b[2J");				//クリアスクリーン[CLS]
						USB_PRINTF("\x1b[0;0H");			//カーソルを0,0に移動
						
						//プッシュスイッチ用処理
						//push_switch = IOex_SWITCH();
			
						/*if(SW_C == 1){
							BEEP();
							break;	
						}*/
					}
				}
				
				break;
				
			case 2:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	X	X	*
				*					*
				*****************************************/	
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					gyro_get_ref();
					BEEP();
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,0);
					log_flag = 0;
					MOT_POWER_OFF;
					BEEP();
					wait_ms(500);
				}
				
				break;
				
			case 3:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	X	X	*
				*					*
				*****************************************/
				//マップの表示
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					gyro_get_ref();
					BEEP();
					log_flag = 1;
					log_timer = 0;
					turn(90,TURN_ACCEL,TURN_SPEED,RIGHT);
					log_flag = 0;
					MOT_POWER_OFF;
					BEEP();
					wait_ms(500);
				}
				
				break;
				
			case 4:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	O	X	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					gyro_get_ref();
					BEEP();
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					straight(SECTION*3,SEARCH_ACCEL,SEARCH_SPEED,0);
					log_flag = 0;
					MOT_POWER_OFF;
					BEEP();
					wait_ms(500);
				}
				
				break;
				
			case 5:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	O	X	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					int i,sum_fl,sum_fr,sum_l,sum_r;
					
					wait_ms(1000);
					tone(NOTE_G4,150);
					
					for(i=0;i<32;i++){
						log_sen[0][i] =  sen_fl.value;
						log_sen[1][i] =  sen_l.value;
						log_sen[2][i] =  sen_r.value;
						log_sen[3][i] =  sen_fr.value;
						sum_fl += log_sen[0][i];
						sum_l  += log_sen[1][i];
						sum_r  += log_sen[2][i];
						sum_fr += log_sen[3][i];
						wait_ms(1);
					}
					sum_fl = sum_fl / 32;
					sum_l  = sum_l / 32;
					sum_r  = sum_r / 32;
					sum_fr = sum_fr / 32;
					
					tone(NOTE_G5,150);
					
					while( SW_C == SW_OFF );
					
					USB_PRINTF("start\n\r");
					USB_PRINTF("%d,",sum_fl);
					USB_PRINTF("%d,",sum_l);
					USB_PRINTF("%d,",sum_r);
					USB_PRINTF("%d\n\r\n\r",sum_fr);
					
					for(i = 0; i < 32; i++){
						
						USB_PRINTF("%d,",i);//time[msec]
						USB_PRINTF("%d,",log_sen[0][i]);
						USB_PRINTF("%d,",log_sen[1][i]);
						USB_PRINTF("%d,",log_sen[2][i]);
						USB_PRINTF("%d\n\r",log_sen[3][i]);
					}

					

					wait_ms(500);		
				}
				
				break;
				
			case 6:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	O	X	*
				*					*
				*****************************************/
				//マップ表示
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					map_copy();
					map_view();
					wait_ms(500);	
				}
				
				break;
				
			case 7:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	O	X	*
				*					*
				*****************************************/
				//ログ出力
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(ad_mode);
					Wait_KeyOff();
					
					USB_PRINTF("time[msec],len_mouse[mm],tar_speed[mm/s],speed[mm/s],Duty_R[%],Duty_L[%],V_battery[mV],tar_degree[deg*10],degree[deg*10],tar_ang_vel[1000*rad/s],ang_vel[1000*rad/s],I_tar_ang_vel[rad],I_ang_vel[rad],ang_acc[1000*rad/ss]\n\r");
					for(i = 0; i < LOG_CNT; i++){
						
						USB_PRINTF("%d,",i);//time[msec]
						USB_PRINTF("%d,",log[0][i]);
						USB_PRINTF("%d,",log[1][i]);
						USB_PRINTF("%d,",log[2][i]);
						USB_PRINTF("%d,",log[3][i]);
						USB_PRINTF("%d,",log[4][i]);
						USB_PRINTF("%d,",log[5][i]);
						USB_PRINTF("%d,",log[6][i]);
						USB_PRINTF("%d,",log[7][i]);
						USB_PRINTF("%d,",log[8][i]);
						USB_PRINTF("%d,",log[9][i]);
						USB_PRINTF("%d,",log[10][i]);
						USB_PRINTF("%d,",log[11][i]);
						USB_PRINTF("%d,",log[12][i]);	
						USB_PRINTF("%d\n\r",log[13][i]);	
					}
					wait_ms(500);	
				}				
				break;
			//mode0~7以外の場合。何もしない。
			default:
				break;
			
		}
		
		//モード切り替え用処理
		if(speed > 0.1 || SW_R == SW_ON){
			if(ad_mode == 7){
				ad_mode = 1;
			}else{
				ad_mode ++;
			}
			for(i = 0; i < 100*1000*10; i++);
			Wait_KeyOff();
			beep(ad_mode);
		}
		
		if(speed < -0.1 || SW_L == SW_ON){
			if(ad_mode == 1){
				ad_mode = 7;
			}else{
				ad_mode --;
		}
			for(i = 0; i < 100*1000*10; i++);
			Wait_KeyOff();
			beep(ad_mode);
		}
		if(flash_time > 0x0FFF){
			flash_time = 0;
			if(flash == 0x08){
				flash = 0x00;
			}else{
				flash = 0x08;
			}
		}
			
		flash_time++;
		LED(ad_mode | flash);
		
		//プッシュスイッチ用処理
		//push_switch = IOex_SWITCH();
		MOT_POWER_OFF;
		
		if(SW_C == SW_ON){
			BEEP();
			Wait_KeyOff();
			break;	
		}
	}
}

