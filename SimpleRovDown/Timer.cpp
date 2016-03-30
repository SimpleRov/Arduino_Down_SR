//*******************************  Библиотеки  ******************************//
#include <Arduino.h>

#include "Timer.h"
//*******************************  /Библиотеки  *****************************//

/// <summary>
/// Переменная хранящее максимальное значение unsigned long
/// </summary>
#define unsignedLongMax 4294967295UL

/// <summary>
/// Переменная хранящее 1, для проверки скорости выполнения цикла
/// </summary>
#define digitalOne 1UL

//********* Функции работы с таймерами на основе millis(), micros() *********//
/// <summary>
/// Получение разницы между 2 uint32_t
/// </summary>
uint32_t GetDifferenceULong(uint32_t BeginTime, uint32_t EndTime)
{
  if (EndTime < BeginTime)
  {
    // Защита от переполнения
    return unsignedLongMax - BeginTime + EndTime + digitalOne;
  }
  else
  {
    return EndTime - BeginTime;
  }
}
//********* /Функции работы с таймерами на основе millis(), micros() ********//
