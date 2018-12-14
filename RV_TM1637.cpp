/*******************
 rvrobotics.ru
 TM1637_ver_1.0
 Ruslan Vorovchenko
*******************/
#include "RV_TM1637.h"
static uint8_t TubeTab[] = {0x3F, 0x06, 0x5B, 0x4F,
                           0x66, 0x6D, 0x7D, 0x07,
                           0x7F, 0x6F, 0x00, 0x40};
RV_TM1637::RV_TM1637(GPIO_TypeDef *_ClkPort, uint16_t _ClkPin, GPIO_TypeDef *_DioPort, uint16_t _DioPin) {
  ClkPort = _ClkPort;
  ClkPin = _ClkPin;
	DioPort = _DioPort;
  DioPin = _DioPin;
	Cmd_SetAddr = STARTADDR;
}
//задержка
void RV_TM1637::delay(uint32_t i) {
	while (i--);
}
//посылка байта
void RV_TM1637::writeByte(int8_t _Data) {
  for (uint8_t i = 0; i < 8; i++) {
    HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_RESET);
    _Data & 0x01 ? HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_SET) : HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_RESET);
    _Data >>= 1;
		delay(I2C_DELAY);
		HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_SET);
		delay(I2C_DELAY);
  }
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_RESET);
	delay(I2C_DELAY);
  HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_RESET);
	
  while (HAL_GPIO_ReadPin(DioPort, DioPin));
	HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_SET);
  delay(I2C_DELAY);
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_RESET);
}
//старт передачи
void RV_TM1637::start(void) {
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_SET);
	delay(I2C_DELAY);
	HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_RESET);
}
//стоп передачи
void RV_TM1637::stop(void) {
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_RESET);
	delay(I2C_DELAY);
	HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_RESET);
	delay(I2C_DELAY);
	HAL_GPIO_WritePin(ClkPort, ClkPin, GPIO_PIN_SET);
	delay(I2C_DELAY);
	HAL_GPIO_WritePin(DioPort, DioPin, GPIO_PIN_SET);
}

//вывод массива цифр по разрядам
void RV_TM1637::display(uint8_t _Data[]) {
  uint8_t Data[4];
  for (uint8_t i = 0; i < 4; i++) {
		lastData[i] = TubeTab[_Data[i]];
		Data[i] = TubeTab[_Data[i]] + PointData;
  }
  sendArray(Data);
}
//вывод массива буквы вида _a , _b... по разрядам
void RV_TM1637::displayByte(uint8_t _Data[]) {
  uint8_t Data[4];
  for (uint8_t i = 0; i < 4; i ++) {
		lastData[i] = _Data[i];
		Data[i] = _Data[i] + PointData;
  }
  sendArray(Data);
}
//вывод цифры в указанный разряд дисплея
void RV_TM1637::display(uint8_t _Addr, int8_t _Data) {
  uint8_t Data;
	lastData[_Addr] = TubeTab[_Data];
	Data = TubeTab[_Data] + PointData;
  sendByte(_Addr, Data);
}
//вывод буквы вида _a , _b... в указанный разряд
void RV_TM1637::displayByte(uint8_t _Addr, int8_t _Data) {
  uint8_t Data;
	lastData[_Addr] = _Data;
	Data = _Data + PointData;
  sendByte(_Addr, Data);
}
//запись байта
void RV_TM1637::sendByte(uint8_t _Addr, int8_t _Data) {
  start();
  writeByte(ADDR_FIXED);
  stop();
  start();
  writeByte(_Addr | 0xC0);
  writeByte(_Data);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//запись данных
void RV_TM1637::sendArray(uint8_t _Data[]) {
  start();
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for (uint8_t i = 0; i < 4; i++)
    writeByte(_Data[i]);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//вывод цифр по разрядам
void RV_TM1637::display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
	uint8_t dispArray[] = {bit0, bit1, bit2, bit3};	
	display(dispArray);
}
//вывод букв вида _a , _b... по разрядам
void RV_TM1637::displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
	uint8_t dispArray[] = {bit0, bit1, bit2, bit3};	
	displayByte(dispArray);
}
//очистка дисплея
void RV_TM1637::clear(void) {
  display(0x00, 0x7F);
  display(0x01, 0x7F);
  display(0x02, 0x7F);
  display(0x03, 0x7F);
  lastData[0] = 0x00;
  lastData[1] = 0x00;
  lastData[2] = 0x00;
  lastData[3] = 0x00;
}
//обновление дисплея
void RV_TM1637::update(void) {
  displayByte(lastData);
}
//настройка яркости 0 - 7	
void RV_TM1637::brightness(uint8_t _Brightness, uint8_t _Data, uint8_t _Addr) {
  Cmd_SetData = _Data;
  Cmd_SetAddr = _Addr;
  Cmd_DispCtrl = 0x88 + _Brightness;
  update();
}
void RV_TM1637::brightness(uint8_t _Brightness) {
  Cmd_DispCtrl = 0x88 + _Brightness;
  update();
}
//включение / выключение точек (POINT_ON / POINT_OFF)
void RV_TM1637::point(bool _PointData) {
  _PointData ? PointData = 0x80 : PointData = 0;
  update();
}
//вывод часов и минут
void RV_TM1637::displayClock(uint8_t _Hours, uint8_t _Minutes) {
	if (_Hours > 99 || _Minutes > 99)
		return;
	uint8_t Time[4];
	!(_Hours / 10) ? Time[0] = 10 : Time[0] = (_Hours / 10);
	Time[1] = _Hours % 10;
	Time[2] = _Minutes / 10;
	Time[3] = _Minutes % 10;
	RV_TM1637::display(Time);
}
//вывод число от -999 до 9999 со знаком
void RV_TM1637::displayInt(int _Value) {
	if (_Value > 9999 || _Value < -999) 
		return;
	bool negative = false;
	uint8_t Digits[4];
	if (_Value < 0) 
		negative = true;	
	_Value = _Value > 0 ? _Value : (-1) * _Value;	
	Digits[0] = (uint8_t) (_Value / 1000);
	Digits[1] = (uint8_t) (_Value % 1000 / 100);
	Digits[2] = (uint8_t) (_Value % 100 / 10);
	Digits[3] = (uint8_t) (_Value % 10);
	if (!negative)
		for (uint8_t i = 0; i < 3; i++)
			if (!Digits[i])
				Digits[i] = 10;
			else
				break;
	else
		for (uint8_t i = 0; i < 3; i++)
			if (Digits[i] == 0) {
				if (!Digits[i + 1])
					Digits[i] = 10;
				else {
					Digits[i] = 11;
					break;
				}
			}
	RV_TM1637::display(Digits);
}
//бегущая строка (массив, количество символов, задержка сдвига)
void RV_TM1637::runningString(uint8_t _Data[], uint8_t _Amount, int _Delay) {
  uint8_t Data[STRING_SIZE];
  for (uint8_t i = 0; i < 4; i++)
    Data[i] = 0x00;
  for (uint8_t i = 0; i < _Amount; i++)
    Data[i + 4] = _Data[i];
  for (uint8_t i = _Amount + 4; i < _Amount + 8; i++)
    Data[i] = 0x00;
  for (uint8_t i = 0; i <= _Amount + 4; i++) {
    displayByte(Data[i], Data[i + 1], Data[i + 2], Data[i + 3]);
    HAL_Delay(_Delay);
  }
}
