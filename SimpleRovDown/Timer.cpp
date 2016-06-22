//*******************************  Библиотеки  ******************************//
#include <Arduino.h>

#include "Timer.h"

#include "FastIO.h"
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
/// Проверка таймера на основе millis
/// </summary>
uint8_t CheckTimerMillis(uint32_t PreviousTimmer, uint32_t Time)
{
  // Если предыдущие значение, больше 0
  if (PreviousTimmer)
  {
    // Получаем время с момента запуска программы и отнимем значение PreviousTimmer (время с начала запуска таймера). После это проверяем, прошло ли установленное время Time
    if (GetDifferenceULong(PreviousTimmer, millis()) >= Time)
    {
      // Возвращаем true
      return 1;
    }
  }

  return 0;
}

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


