
typedef struct
{
	short value;		//現在の値 SEN
	short p_value;		//１mS過去の値 
	short error;		//value - ref
	short ref;		//リファレンス値
	short th_wall;		//壁があるか否かの閾値
	short th_control;	//制御をかけるか否かの閾値
}t_sensor;			//センサ構造体
