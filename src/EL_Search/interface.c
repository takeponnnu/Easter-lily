#include "iodefine.h"
#include "mathf.h"
#include "init.h"
#include "spi.h"
//#include "i2c.h"
#include "parameters.h"
#include "glob_var.h"
#include "portdef.h"

extern wait_ms(int wtime);


void LED(short led_data){
	
	LED1 = led_data&0x01;
	LED3 = (led_data>>1)&0x01;
	LED4 = (led_data>>2)&0x01;
	LED2 = (led_data>>3)&0x01;
}

void BEEP(void){
	int i,j;
	//BUZZER = 0;
	for(j=0 ; j<100 ; j++){
		PORTB.PODR.BIT.B3 = 1;
		for(i = 0; i < 100; i++);
		PORTB.PODR.BIT.B3 = 0;
		for(i = 0; i < 100; i++);
	}
	//BUZZER = 1;
	
}

void BEEP_MULT(short n){
	int i,j,k;
	for(k = 0; k < n+1; k++){
		for(j=0 ; j<1000*0.1 ; j++){
			PORTB.PODR.BIT.B3 = 1;
			for(i = 0; i < 100*10*1; i++);
			PORTB.PODR.BIT.B3 = 0;
			for(i = 0; i < 100*10*1; i++);
		}
		wait_ms(100);
	}
}

void tone(short freq , short note){
	SET_BUZZER_FREQ(freq);
	ENABLE_BUZZER;
	wait_ms(note);
	DISABLE_BUZZER;
}

void mode_change( char* mode){
	int i;
	//ƒ‚[ƒhØ‚è‘Ö‚¦—pˆ—
	if(SW_R == SW_ON || speed_r > 0.1){
		if(*mode == 15){
			*mode = 0;
		}else{
			*mode= *mode + 1;
		}
		BEEP(); 
		LED(*mode);
		for(i = 0; i < 100*1000*50; i++);
	}

	if(SW_L == SW_ON || speed_r < -0.1){
		if(mode == 0){
			*mode = 15;
		}else{
			*mode = *mode -1;
		}
		BEEP(); 
		LED(*mode);
		for(i = 0; i < 100*1000*50; i++);
	}	
}

void Wait_KeyOff(void){
	wait_ms(200);
	
	while(( SW_L == SW_ON )||( SW_R == SW_ON )||(SW_C == SW_ON));
}
