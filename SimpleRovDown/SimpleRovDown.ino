/*
Sketch uses 11 430 bytes (39%) of program storage space. Maximum is 28 672 bytes.
Global variables use 538 bytes (21%) of dynamic memory, leaving 2 022 bytes for local variables. Maximum is 2 560 bytes.
*/

//*******************************  Библиотеки  *******************************//
#include "Arduino.h"

#include "Protocol.h"

#include "Config.h"

#include "Def.h"

#include "Type.h"

#include "GlobalVar.h"

#include "FastIO.h"

//#include "ADC.h"

//#include "TWI.h"

#include "Timer.h"

#include "Sensors.h"

#include <util/delay.h>
//*******************************  /Библиотеки  ******************************//

//********************** Объявление переменных, констант *********************//
// Переменная хранящая предыдущие значение первого массива кнопок джойстика.
uint8_t previousBFirst = 0;      

// Переменная хранящая предыдущие значение второго массива кнопок джойстика.
uint8_t previousBSecond = 0; 

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
  #ifdef DEBUGGING_THROUGH_UART
    Serial.begin(115200);
    while (!Serial) 
    {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  #endif   

  #ifdef DEBUGGING_THROUGH_UART
    SetSSerial(&Serial);
  
    DEBUG_PRINTLN(F("Setup Begin"));
  #endif 

  // Инициализация UART.
  InitUart();

  // Инициализация датчиков.
  InitSensors();
  
  // Немного подождем, вдруг, что-то не успело инициализировать.
  _delay_ms(1000);

  #ifdef DEBUGGING_THROUGH_UART
    DEBUG_PRINTLN(F("Setup End"));
  #endif
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
    #ifdef DEBUGGING_THROUGH_UART
      DEBUG_PRINTLN(F("Data Exist"));
    #endif
    
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
  AdcSensorsSendDataInStruct();
  
  // Тест MS580330BA.
  Ms580330BaSendDataInStruct();

  #ifdef DEBUG_SPEED_CYCLE
    // Определяем скорость работы.
    timeCycle = GetDifferenceULong(timeCycleBegin, micros());
    // Если текущее значение больше, последнего максимального, отображаем его.
    if (timeCycle > timeCycleMax)
    {
      timeCycleMax = timeCycle;

      #ifdef DEBUGGING_THROUGH_UART
        DEBUG_PRINT("Max - ");
        DEBUG_PRINTLN(timeCycleMax);
      #endif
    }
  #endif
}
//************************** Функции ответа Rov по UART **********************//
void RovSendAnswer(void)
{ 
  rovDataS.errore = 1;    

  SendStruct((uint8_t*)&rovDataS, 1, 2);
}
//************************** /Функции ответа Rov по UART *********************//
