//*******************************  Библиотеки  *******************************//
#include "Arduino.h"

#include "Protocol.h"

#include "Config.h"

#include "Def.h"

#include "Type.h"

#include "GlobalVar.h"

#include "FastIO.h"

#include <util/delay.h>
//*******************************  /Библиотеки  ******************************//

#ifndef DEBUG_PRINT
  #define DEBUG_PRINT(x) DebugSerial->print(x)
#endif

#ifndef DEBUG_PRINTF
  #define DEBUG_PRINTF(x, y) DebugSerial->print(x, y)
#endif

#ifndef DEBUG_PRINTLN
  #define DEBUG_PRINTLN(x) DebugSerial->println(x)
#endif

#ifndef DEBUG_PRINTLNF
  #define DEBUG_PRINTLNF(x, y) DebugSerial->println(x, y)
#endif

//********************** Объявление переменных, констант *********************//
// Переменная хранящая предыдущие значение первого массива кнопок джойстика.
static uint8_t previousBFirst = 0;      

// Переменная хранящая предыдущие значение второго массива кнопок джойстика.
static uint8_t previousBSecond = 0; 

// Структура для хранения данных MS5803_30BA.
Ms580330Ba ms580330Ba;
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
  
  // Немного подождем, вдруг, что-то не успело инициализировать.
  _delay_ms(1000);

  DEBUG_PRINTLN(F("Setup End"));
}

/// <summary>
/// Основной цикл.
/// </summary>
void loop() 
{
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
}

//************************** Функции ответа Rov по UART **********************//
void RovSendAnswer(void)
{ 
  rovDataS.errore = 1;    

  SendStruct((uint8_t*)&rovDataS, 1, 2);
}
//************************** /Функции ответа Rov по UART *********************//
