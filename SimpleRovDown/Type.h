#ifndef Type_H_
#define Type_H_

//*******************************  Библиотеки  ******************************//
// Класс Arduino.
#include "Arduino.h"
//******************************* /Библиотеки  ******************************//
typedef struct 
{
  uint8_t bfirst, bsecond, statuswork;
  uint8_t lx, ly, rx, ry;
} ps2Struct;  

typedef struct 
{
  uint8_t errore;
} rovDataStruct; 

// Структура для хранения данных MS5803_30BA. 
// twimStep используется для хранения последней операции.
typedef struct 
{
  uint8_t twimStep, valueGet;
  uint32_t pressureRaw, temperatureRaw;
  double pressureValue, temperatureValue;
} Ms580330Ba;


#endif /* Type_H_ */
