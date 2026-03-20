
#ifndef _PARAMETER
#define _PARAMETER

#include "static_parameters.h"

//物理的なパラメータ
#define TIRE_DIAMETER	(24.65)				//タイヤの直径	[mm]
#define TIRE_RADIUS	(TIRE_DIAMETER/2.0)			//タイヤの半径	[mm]
#define MMPP 		(TIRE_DIAMETER*PI)/(ENC_RES_MAX*5)	//エンコーダ1パルスあたりに進む距離[mm](TIRE_DIAMETER*PI)/(ENC_MAX) ギア比もかけてる 8:40 = 1:5
#define ENC_RES_MAX	(2048)				// 軸が1回転する時に数えられるパルス量 512*4逓倍=2048
#define ENC_RES_HALF	(1024)

#define V_ref		3.8				//モータ制御の基準電圧		HMから変えてない

//ログ用のパラメータ
#define LOG_CNT		1000				//ログをとる個数。 1mmsで取得しているので、取得時間[s]はこの数の1000分の1

//センサ関連パラメータ
#define WAITLOOP_SLED	180				//LEDを光らせてからAD変換を開始するまでの時間稼ぎ用定数

#define REF_SEN_R	1682				//マウスを迷路中央に置いた時のセンサの値
#define REF_SEN_L	1828				//マウスを迷路中央に置いた時のセンサの値

#define TH_SEN_R	1000				//壁があるか否かの閾値	車体を区画の左へ寄せた時のセンサ値(壁あり)
#define TH_SEN_L	1000				//壁があるか否かの閾値	車体を区画の右へ寄せた時のセンサ値(壁あり)
#define TH_SEN_FR	600				//壁があるか否かの閾値	
#define TH_SEN_FL	480				//壁があるか否かの閾値

#define RIGHT_90	914				//前壁での距離調整
#define LEFT_90		701				//見てるタイミングの値をいれる

#define CONTH_SEN_R	TH_SEN_R			//制御をかけるか否かの閾値
#define CONTH_SEN_L	TH_SEN_L			//制御をかけるか否かの閾値
#define CON_WALL_KP	(5.0)				//壁センサによる姿勢制御の比例制御の比例定数	10.0

#endif
