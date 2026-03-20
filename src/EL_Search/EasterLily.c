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
#include "sci.h"
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


extern void wait_ms(int wtime);
extern void adjust(void);
extern void melody( int x );
extern void beep( int x );

extern void USB_PRINTF(char *format, ...);
extern int get_encoder_r(void);
extern int get_encoder_l(void);

void main(void)
{

	init_all();
	unsigned long i = 0;
	

	
	//?��u?��U?��[
	//BEEP();
	melody(0);
	//tone (NOTE_C5 , 1000);
	//?��ŏ�?��?��0?��?��?��Ă�?��?��
	speed_r=0;
	speed_l=0;
	
	//?��N?��?��?��?��?���??��?��O?��͂Ƃ�Ȃ�
	log_flag = 0;
	short mode = 1;
	while(1){
		I_tar_ang_vel = 0;
		I_ang_vel = 0;
		I_tar_speed = 0;
		I_speed = 0;

		switch(mode){
			
			case 1:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	X	X	*
				*					*
				*****************************************/
				
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					
					degree = 0;
					timer = 0;
					log_timer = 0;
					gyro_get_ref();
					//BEEP();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					search_adachi(GOAL_X,GOAL_Y);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					//BEEP();
					wait_ms(50);
					melody(2);
					//BEEP();//?��S?��[?��?��?��?��?��?��?��?��?��Ƃ�?��A?��s?��[?��?��
					wait_ms(50);
					//BEEP();//?��S?��[?��?��?��?��?��?��?��?��?��Ƃ�?��A?��s?��[?��?��
					len_mouse = 0;
					melody(2);
					search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					map_write();
					log_flag = 0;
					melody(2);
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
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					//BEEP();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,1);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					melody(2);
					wait_ms(100);
					melody(3);
					len_mouse = 0;
					search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					map_write();
					log_flag = 0;
					BEEP();
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
			//?��T?��?��?��s?��?��?��̂�
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					degree = 0;
					timer = 0;
					log_timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					search_adachi(GOAL_X,GOAL_Y);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					//BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();
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
			//?��ŒZ?��̂� spd 1.0 acc 2.0
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,1);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();
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
			// ?��ŒZ spd 1.2 acc 2 
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
	
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,2);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();		
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
			//?��ŒZ spd 1.2 acc 2.2
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,3);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();
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
			//?��ŒZ spd 1.5 acc 2.0
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,4);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();	
				}
				
				break;
				
			case 8:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	X	O	*
				*					*
				*****************************************/
			//?��ŒZ?��@spd 1.5 acc 2.2
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					melody(3);
					mypos.x = mypos.y = 0;			//?��?��?��W?��?��?��?��?��?��?��?��
					mypos.dir = north;			//?��?��?��p?��?��?��?��?��?��?��?��
					log_flag = 1;
					log_timer = 0;
					len_mouse = 0;
					fast_run(GOAL_X,GOAL_Y,5);		//?��S?��[?��?��?��܂ő�?��?��?��@
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��S?��[?��?��?��?��?��?��?��?��180?��x?��?��]?��?��?��?��
					mypos.dir = (mypos.dir+6) % 4;		//?��?��?��p?��?��?��X?��V
					map_write();
					BEEP();
					wait_ms(100);
					melody(2);
					//search_adachi(0,0);			//?��X?��^?��[?��g?��n?��_?��܂ő�?��?��?��@?��ŋA?��?��?��Ă�?��?��
					//turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//?��A?��?��?��Ă�?��?��?��?��180?��x?��?��]	
					MOT_POWER_OFF;
					//map_write();
					log_flag = 0;
					BEEP();
				}
				
				break;
				
			case 9:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	X	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					wait_ms(500);		
				}
				
				break;
				
			case 10:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	X	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					while(1){
						int i;
						i = check_wall();
						LED(i);
						wait_ms(1);
					}

					wait_ms(500);		
				}

				break;
				
			case 11:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	X	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					wait_ms(500);			
				}
				
				break;
				
			case 12:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	O	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					wait_ms(500);		
				}
				
				break;
				
				
			case 13:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	O	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();

					wait_ms(500);		
				}
				
				break;
				
			case 14:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	O	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4 || SW_C == SW_ON){
					beep(mode);
					Wait_KeyOff();
					
					straight(SECTION*15,SEARCH_ACCEL,SEARCH_SPEED,0);

					wait_ms(500);		
				}
				
				break;
				
			case 15:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	O	O	*
				*					*
				*****************************************/
			
				//?��Z?��?��?��T?��[?��̑O?��Ɏ�?��?��?��?��?��?��?��?��ăX?��^?��[?��g
				if((sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4) || SW_C == SW_ON){
					beep(mode);
					while(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4);
					Wait_KeyOff();
					adjust();
					
					BEEP();
					wait_ms(500);
				}
				
				break;
				
			//mode0~15?��ȊO?��̏ꍇ?��B?��?��?��?��?��?��?��Ȃ�?��B
			default:
				break;
			
		}
		
		//?��?��?��[?��h?��؂�ւ�?��p?��?��?��?��
		if(len_mouse_int > 1000 || SW_R == SW_ON){
			if(mode == 15){
				mode = 1;
			}else{
				mode ++;
			}
			len_mouse_int = 0;
			Wait_KeyOff();
			for(i = 0; i < 100*100*10; i++);
			beep(mode);
		}
		
		if(len_mouse_int < -1000 || SW_L == SW_ON){
			if(mode == 1){
				mode = 15;
			}else{
				mode --;
		}
			len_mouse_int = 0;
			Wait_KeyOff();
			for(i = 0; i < 100*100*10; i++);
			beep(mode); 
		}
		LED(mode);
		
		//?��v?��b?��V?��?��?��X?��C?��b?��`?��p?��?��?��?��
		//push_switch = IOex_SWITCH();
		MOT_POWER_OFF;
	
	}
	

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
