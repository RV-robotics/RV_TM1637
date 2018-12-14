/*******************
 rvrobotics.ru
 TM1637_ver_1.0
 Ruslan Vorovchenko
*******************/
#ifndef RV_TM1637_h
#define RV_TM1637_h

#include <stdint.h>
#include "stm32f4xx_hal.h"

class RV_TM1637 {
  public:
		RV_TM1637(GPIO_TypeDef *_ClkPort, uint16_t _ClkPin, GPIO_TypeDef *_DioPort, uint16_t _DioPin);//инициализация
	
    void display(uint8_t _Data[]);//вывод массива цифр по разрядам
    void display(uint8_t _Addr, int8_t _Data);//вывод цифры в указанный разряд дисплея
		void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);//вывод цифр по разрядам
	
		void displayByte(uint8_t _Data[]);//вывод массива буквы вида _a , _b... по разрядам
		void displayByte(uint8_t _Addr, int8_t _Data);//вывод буквы вида _a , _b... в указанный разряд
		void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);//вывод букв вида _a , _b... по разрядам
	
    void displayClock(uint8_t _Hours, uint8_t _Minutes);//вывод часов и минут
	
		void displayInt(int _Value);//вывод число от -999 до 9999 со знаком
	
		void runningString(uint8_t _Data[], uint8_t _Amount, int _Delay);//бегущая строка (массив, количество символов, задержка сдвига)
	
		void clear(void);//очистка дисплея
	
		void point(bool _PointData);//включение / выключение точек (POINT_ON / POINT_OFF)

		void brightness(uint8_t _Brightness, uint8_t _Data, uint8_t _Addr);//настройка яркости 0 - 7	
		void brightness(uint8_t _Brightness);

  private:
    uint8_t lastData[4];//текущие данные
		void update();//обновление дисплея
    void writeByte(int8_t _Data);//посылка байта
    void start(void);//старт передачи
    void stop(void);//стоп передачи
	
		void delay(uint32_t i);//задержка
	
		void sendByte(uint8_t _Addr, int8_t _Data);//запись байта
		void sendArray(uint8_t _Data[]);//запись данных

    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;
		uint8_t PointData;
  
		GPIO_TypeDef *ClkPort;
    uint16_t ClkPin;
		GPIO_TypeDef *DioPort;
    uint16_t DioPin;
};

/***константы для TM1637***/
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define STARTADDR  0xc0

/***константа задержки***/
#define I2C_DELAY 200

/***константа максимальной длины бегущей строки***/
#define STRING_SIZE 50

/***константы для точек***/
#define POINT_ON  true
#define POINT_OFF false

/***символы***/
#define _a 0x5F
#define _b 0x7C
#define _c 0x58
#define _d 0x5E
#define _e 0x7B
#define _f 0x71
#define _g 0x3D
#define _h 0x74
#define _i 0x10
#define _j 0x0E
#define _k 0x7A
#define _l 0x06
#define _m 0x15
#define _n 0x54
#define _o 0x5C
#define _p 0x73
#define _q 0x67
#define _r 0x50
#define _s 0x6D
#define _t 0x78
#define _u 0x1C
#define _v 0x72
#define _w 0x7E
#define _x 0x64
#define _y 0x6E
#define _z 0x5B

#define _A 0x77
#define _B 0x7F
#define _C 0x39
#define _D 0x1F
#define _E 0x79
#define _F 0x71
#define _G 0x3D
#define _H 0x76
#define _I 0x30
#define _J 0x1E
#define _K 0x7A
#define _L 0x38
#define _M 0x15
#define _N 0x37
#define _O 0x3F
#define _P 0x73
#define _Q 0x6B
#define _R 0x33
#define _S 0x6D
#define _T 0x07
#define _U 0x3E
#define _V 0x72
#define _W 0x7E
#define _X 0x64
#define _Y 0x6E
#define _Z 0x5B

/***цифры***/
#define _0 0x3F
#define _1 0x06
#define _2 0x5B
#define _3 0x4F
#define _4 0x66
#define _5 0x6D
#define _6 0x7D
#define _7 0x07
#define _8 0x7F
#define _9 0x6F

/***минус, нижнее подчеркивание, знак равенства, пробел, градус Цельсия***/
#define _dash 0x40
#define _under 0x08
#define _equal 0x48
#define _empty 0x00
#define _degree 0x63

#endif
