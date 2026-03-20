
#include "portdef.h"
#include "iodefine.h"

void init_all(void);	//全部初期化(全部を呼び出す)
void init_clock(void);	//CPUの動作周波数を設定
void init_io(void);	//IOポート初期化
void init_cmt(void);	//CMT初期化
void init_mtu(void);	//MTU設定
void init_adc(void);	//AD初期化
void gyro_get_ref(void);

