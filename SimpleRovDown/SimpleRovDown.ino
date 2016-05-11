//*******************************  Библиотеки  *******************************//
#include "Arduino.h"

#include "Protocol.h"

#include "Config.h"

#include "Def.h"

#include "Type.h"

#include "GlobalVar.h"

#include "FastIO.h"

#include "ADC.h"

#include "TWI.h"

#include "Timer.h"

#include <util/delay.h>
//*******************************  /Библиотеки  ******************************//

//********************** Объявление переменных, констант *********************//
// Переменная хранящая предыдущие значение первого массива кнопок джойстика.
static uint8_t previousBFirst = 0;      

// Переменная хранящая предыдущие значение второго массива кнопок джойстика.
static uint8_t previousBSecond = 0; 

// Время после смены опорного напряжения и получением 1 значения.
uint32_t adcChangeRefT = 2000;

// Переменные для определения максимального времени цикла.
uint32_t timeCycleBegin = 0;
uint32_t timeCycle = 0;
uint32_t timeCycleMax = 0;
//********************** /Объявление переменных, констант ********************//

//****************************** Основные функции ****************************//
/// <summary>
/// Основная функция main.
/// </summary>
int main(void)
{
    init();

    #if defined(USBCON)
      USBDevice.attach();
    #endif
   
    setup();
   
    for (;;) 
    {
      loop();
      // В проекте не используется стандартная библиотека Serial. 
      //if (serialEventRun) serialEventRun();
    }
       
    return 0;
}

/// <summary>
/// Настройка при запуске arduino
/// </summary>
void setup()
{  
  // Инициализация UART.
  InitUart();

  Serial.begin(115200);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  SetSSerial(&Serial);

  DEBUG_PRINTLN(F("Setup Begin"));

  // Инициализация ADC.
  AdcInit();
  
  // Немного подождем, вдруг, что-то не успело инициализировать.
  _delay_ms(1000);

  DEBUG_PRINTLN(F("Setup End"));
}

/// <summary>
/// Основной цикл.
/// </summary>
void loop() 
{
  // Начальное время цикла.
  timeCycleBegin = micros();
  
  CheckUart();

  // Пришли данные с джойстика.
  if (ps2S.statuswork == 1)
  { 
    DEBUG_PRINTLN(F("Data Exist"));

    previousBFirst = ps2S.bfirst;
    previousBSecond = ps2S.bsecond;
        
    ps2S.statuswork = 2;  
    ps2S.bfirst = ps2S.bsecond = 0;
  }

  if (ps2S.statuswork == 2)
  {
    RovSendAnswer();
    ps2S.statuswork = 3;
  }

  // Тест ADC.
  static uint8_t stepAdcSelect = 2;
  if (!stepAdcSelect)
  {
    SetAdcPin(0);
    if (AdcReadyToRead())
    {
      uint16_t adcValue = GetAdcValue();
      DEBUG_PRINT("Value A0 - ");    
      DEBUG_PRINTLN(adcValue);

      stepAdcSelect = 1;
    }
  }
  
  if (stepAdcSelect == 1)
  {
    SetAdcPin(1);
    if (AdcReadyToRead())
    {
      uint16_t adcValue = GetAdcValue();
      DEBUG_PRINT("Value A1 - ");    
      DEBUG_PRINTLN(adcValue);

      stepAdcSelect = 2;
    }
  }

  // Время установки нового опорного напряжения.
  static uint32_t adcChangeRefPreviousT = 0; 
  if (stepAdcSelect == 2)
  {
    SetAdcPin(AVR_ADC_TEMPERATURE_SENSOR);
    if (AdcReadyToRead())
    {
      uint16_t adcValue = GetAdcValue();
      DEBUG_PRINT("Value T - ");    
      DEBUG_PRINTLN((adcValue - 273));

      // Сохраняем время установки нового опорного напряжения.
      adcChangeRefPreviousT = micros();

      stepAdcSelect = 2;
    }
  }

  if (stepAdcSelect == 3 && GetDifferenceULong(adcChangeRefPreviousT, micros()) > adcChangeRefT)
  {
    SetAdcPin(AVR_ADC_TEMPERATURE_SENSOR);
    if (AdcReadyToRead())
    {
      uint16_t adcValue = GetAdcValue();
      DEBUG_PRINT("Value T - ");    
      DEBUG_PRINTLN((adcValue - 273));
  
      stepAdcSelect = adcChangeRefPreviousT = 0;
    }
  }

  // Определяем скорость работы.
  timeCycle = GetDifferenceULong(timeCycleBegin, micros());
  // Если текущее значение больше, последнего максимального, отображаем его.
  if (timeCycle > timeCycleMax)
  {
    timeCycleMax = timeCycle;
  
    DEBUG_PRINT("Max - ");
    DEBUG_PRINTLN(timeCycleMax);
  }
}

//************************** Функции ответа Rov по UART **********************//
void RovSendAnswer(void)
{ 
  rovDataS.errore = 1;    

  SendStruct((uint8_t*)&rovDataS, 1, 2);
}
//************************** /Функции ответа Rov по UART *********************//
