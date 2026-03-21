

#ifdef PORTDEF
	
#else

#define PORTDEF

#define BUZZER	(PORTB.PODR.BIT.B3)

#define LED0	(PORTA.PODR.BIT.B3)
#define LED1	(PORTC.PODR.BIT.B3)
#define LED2	(PORTB.PODR.BIT.B7)
#define LED3	(PORT5.PODR.BIT.B5)
#define LED4	(PORT5.PODR.BIT.B4)

#define SW_R	(PORT3.PIDR.BIT.B5)
#define SW_C	(PORT0.PIDR.BIT.B5)
#define SW_L	(PORTB.PIDR.BIT.B5)

#define S_LED	(PORTB.PODR.BIT.B6)		//左センサLED
//#define SLED_R	(PORT0.PODR.BIT.B5)		//右センサLED
//#define SLED_FL	(PORT2.PODR.BIT.B7)		//左前センサLED
//#define SLED_FR	(PORTB.PODR.BIT.B5)		//右前センサLED

#define MOT_OUT_R	(MTU4.TGRA)
#define MOT_OUT_L	(MTU3.TGRD)

#define MOT_POWER	(PORTB.PODR.BIT.B1)
#define MOT_POWER_ON	MOT_POWER = 1		// ステッピングモータを励磁
#define MOT_POWER_OFF	MOT_POWER = 0		// ステッピングモータを脱力


#define MOT_CWCCW_R1	(PORTA.PODR.BIT.B0)
#define MOT_CWCCW_R2	(PORTA.PODR.BIT.B4)
#define MOT_CWCCW_L1	(PORTB.PODR.BIT.B0)
#define MOT_CWCCW_L2	(PORTA.PODR.BIT.B6)

#define COMMON_ANOD	(PORTE.PODR.BIT.B3)

//#define BLED0	(PORTB.PODR.BIT.B1)		//バッテリ電圧表示LED 青
//#define BLED1	(PORTA.PODR.BIT.B3)		//バッテリ電圧表示LED 赤

#endif
