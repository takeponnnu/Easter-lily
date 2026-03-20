#include "portdef.h"

#define SET_BUZZER_FREQ(f)	MTU0.TGRB=(unsigned short)(12000000/(f));MTU0.TGRA=(unsigned short)(6000000/(f))	//ブザーの発振周波数を算出して、設定

#define ENABLE_BUZZER		PORTB.PMR.BIT.B3=1;MTU.TSTR.BIT.CST0=1	//ブザーの発振を開始
#define DISABLE_BUZZER		PORTB.PMR.BIT.B3=0;MTU.TSTR.BIT.CST0=0	//ブザーの発振を停止

#define TIRE_CIRCUIT	(PI*TIRE_DIAMETER)			//タイヤの円周を直径から算出(PIは円周率)
#define SPEED2GREG(v)	(7500/(((v)/TIRE_CIRCUIT)))		//スピードからジェネラルレジスタの値を計算

#define LEN2STEP(l)	(2*400*(l)/TIRE_CIRCUIT)		//走行距離(mm)からステッピングモータのステップ数を算出
#define STEP2LEN(s) 	(TIRE_CIRCUIT*(s)/(2*400))		//ステッピングモータのステップ数から走行距離を算出

#define CONV_SEN2WALL(w) ((w) ? WALL : NOWALL)			//センサ情報から壁情報へ変換

#define NOTE_C4		523
#define NOTE_Cs4	554	// C#4
#define NOTE_D4		587
#define NOTE_Ds4	622
#define NOTE_E4		659
#define NOTE_F4		698
#define NOTE_Fs4	740
#define NOTE_G4		784
#define NOTE_Gs4	831
#define NOTE_A4		880
#define NOTE_As4	932
#define NOTE_B4		988
#define NOTE_C5		1047
#define NOTE_Cs5	1109
#define NOTE_D5		1175
#define NOTE_Ds5	1245
#define NOTE_E5		1319
#define NOTE_F5		1397
#define NOTE_Fs5	1480
#define NOTE_G5		1568
#define NOTE_Gs5	1661
#define NOTE_A5		1760
#define NOTE_As5	1865
#define NOTE_B5		1976
#define NOTE_C6		2093
#define NOTE_D6		2349
#define NOTE_E6		2637
#define NOTE_Gs6	3322
#define NOTE_A6		3520