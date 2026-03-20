#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#include "init.h"
#include "parameters.h"
#include "glob_var.h"
#include "portdef.h"
#include "interface.h"

extern wait_ms(int wtime);

void melody( int x ){
	
	switch( x ){
		case 0 :// tempo 100 1/4 600msec
			LED(9);
			tone(NOTE_G4,150);
			LED(0);
			wait_ms(150);
			LED(9);
			tone(NOTE_G4,150);
			LED(0);
			wait_ms(150);
			break;
			
		case 1 :// 
			LED(9);
			tone(NOTE_G4,150);
			LED(0);
			wait_ms(150);
			break;
			
		case 2 :// freedom dive tempo 222 1/4 270.27
			LED(9);
			tone(NOTE_C5,75);
			wait_ms(425);
			LED(15);
			tone(NOTE_C5,75);
			wait_ms(425);
			LED(0);
			tone(NOTE_C5,75);
			wait_ms(425);
			LED(15);
			tone(NOTE_C6,500);
			break;
			
		case 3:// start appeal
			LED(9);
			tone(NOTE_C5,75);
			wait_ms(425);
			LED(15);
			tone(NOTE_C5,75);
			wait_ms(425);
			LED(15);
			tone(NOTE_C6,500);
			break;
	}
	
}

void beep( int x ){
	switch(x){
		case 1: // ƒh
			tone(NOTE_C4,200);
			break;
		
		case 2: 
			tone(NOTE_D4,200);
			break;
		
		case 3: 
			tone(NOTE_E4,200);
			break;
		
		case 4: 
			tone(NOTE_F4,200);
			break;
		
		case 5: 
			tone(NOTE_G4,200);
			break;
		
		case 6: 
			tone(NOTE_A4,200);
			break;
			
		case 7: 
			tone(NOTE_B4,200);
			break;
			
		case 8: 
			tone(NOTE_C5,200);
			break;
			
		case 9: 
			tone(NOTE_D5,200);
			break;
			
		case 10: 
			tone(NOTE_E5,200);
			break;
			
		case 11: 
			tone(NOTE_F5,200);
			break;
			
		case 12: 
			tone(NOTE_G5,200);
			break;
		
		case 13: 
			tone(NOTE_A5,200);
			break;
		
		case 14: 
			tone(NOTE_B5,200);
			break;
		
		case 15: 
			tone(NOTE_C6,200);
			break;
			
	}
	
}